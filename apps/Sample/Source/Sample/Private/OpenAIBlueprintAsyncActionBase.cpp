// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenAIBlueprintAsyncActionBase.h"
#include "Dom/JsonValue.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UOpenAIBlueprintAsyncActionBase* UOpenAIBlueprintAsyncActionBase::SampleOpenAI(const EOpenAIApiType& OpenAIApiType)
{
	UOpenAIBlueprintAsyncActionBase* OpenAIAction = NewObject<UOpenAIBlueprintAsyncActionBase>();
	OpenAIAction->OpenAIApiKey = "";
	OpenAIAction->AzureOpenAISubscriptionKey = "";
	OpenAIAction->AzureOpenAIRegion = "";
	OpenAIAction->OpenAIApiType = OpenAIApiType;
	return OpenAIAction;
}

void UOpenAIBlueprintAsyncActionBase::Activate()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UOpenAIBlueprintAsyncActionBase::HandleRequest);

	if (OpenAIApiType == EOpenAIApiType::ChatGPT)
	{
		// ChatGPT API
		Request->SetVerb("POST");
		Request->SetURL("https://your_azure_openai_resource_name.openai.azure.com/openai/deployments/your_azure_openai_deployment_name/completions?api-version=2022-12-01");
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetHeader(TEXT("api-key"), TEXT("your_azure_openai_subscription_key"));

		TArray<FString> SampleInputs = {
			"Clippy is an endearing and helpful digital assistant, designed to make using Microsoft Office Suite of products more efficient and user-friendly. With his iconic paperclip shape and friendly personality, Clippy is always ready and willing to assist users with any task or question they may have. His ability to anticipate and address potential issues before they even arise has made him a beloved and iconic figure in the world of technology, widely recognized as an invaluable tool for productivity.",
			"What is Azure Cognitive Search?"
		};
		FString Prompt = UOpenAIBlueprintAsyncActionBase::ComposePrompt(SampleInputs);
		FString RequestBody = FString::Printf(TEXT("{\"prompt\": \"%s\",\"temperature\": 0.5,\"top_p\": 0.95,\"frequency_penalty\": 0,\"presence_penalty\": 0,\"max_tokens\": 800,\"stop\": [\"<|im_end|>\"]}"), *Prompt);

		Request->SetContentAsString(RequestBody);
		Request->ProcessRequest();
	}
	else if (OpenAIApiType == EOpenAIApiType::DALLE)
	{
		// DALLE API
		Request->SetVerb("POST");
		Request->SetURL("https://api.openai.com/v1/images/generations");
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetHeader(TEXT("Authorization"), TEXT("Bearer your_openai_api_key"));

		FString RequestBody = FString::Printf(
			TEXT("{\"prompt\":\"%s\",\"n\":%d,\"size\":\"%s\"}"),
			TEXT("a white siamese cat"),
			1,
			TEXT("256x256")
		);

		Request->SetContentAsString(RequestBody);
		Request->ProcessRequest();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown OpenAIApiType"));
	}
}

void UOpenAIBlueprintAsyncActionBase::HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess)
{
	if (bSuccess && HttpResponse.IsValid())
	{
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogTemp, Display, TEXT("Web API Response: %s"), *ResponseString);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (OpenAIApiType == EOpenAIApiType::ChatGPT)
			{
				const TArray<TSharedPtr<FJsonValue>>* ChoicesArrayPtr = nullptr;
				if (JsonObject->TryGetArrayField("choices", ChoicesArrayPtr) && ChoicesArrayPtr != nullptr)
				{
					const TArray<TSharedPtr<FJsonValue>>& ChoicesArray = *ChoicesArrayPtr;
					if (ChoicesArray.Num() > 0 && ChoicesArray[0]->Type == EJson::Object)
					{
						TSharedPtr<FJsonObject> FirstChoiceObject = ChoicesArray[0]->AsObject();
						FString FirstChoiceText;
						if (FirstChoiceObject->TryGetStringField("text", FirstChoiceText))
						{
							OnSuccess.Broadcast(FirstChoiceText);
							return;
						}
					}
				}
			}
			else if (OpenAIApiType == EOpenAIApiType::DALLE)
			{
				const TArray<TSharedPtr<FJsonValue>>* DataArrayPtr = nullptr;
				if (JsonObject->TryGetArrayField("data", DataArrayPtr) && DataArrayPtr != nullptr)
				{
					const TArray<TSharedPtr<FJsonValue>>& DataArray = *DataArrayPtr;
					if (DataArray.Num() > 0 && DataArray[0]->Type == EJson::Object)
					{
						TSharedPtr<FJsonObject> FirstDataObject = DataArray[0]->AsObject();
						FString DataUrl;
						if (FirstDataObject->TryGetStringField("url", DataUrl))
						{
							OnSuccess.Broadcast(DataUrl);
							return;
						}
					}
				}
			}
		}
	}

	OnFailure.Broadcast("");
}

FString UOpenAIBlueprintAsyncActionBase::ComposePrompt(const TArray<FString>& Inputs)
{
	FString Prompt;
	for (int i = 0; i < Inputs.Num(); i++) {
		if (i % 2 == 0) {
			Prompt += "<|im_start|>system\\n" + Inputs[i] + "\\n<|im_end|>\\n";
		}
		else {
			Prompt += "<|im_start|>user\\n" + Inputs[i] + "\\n<|im_end|>\\n";
		}
	}
	return Prompt;
}
