// Fill out your copyright notice in the Description page of Project Settings.

#include "WebApiBlueprintAsyncActionBase.h"
#include "Dom/JsonValue.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UWebApiBlueprintAsyncActionBase* UWebApiBlueprintAsyncActionBase::SampleWebApi()
{
	UWebApiBlueprintAsyncActionBase* WebApiAction = NewObject<UWebApiBlueprintAsyncActionBase>();
	return WebApiAction;
}

void UWebApiBlueprintAsyncActionBase::Activate()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UWebApiBlueprintAsyncActionBase::HandleRequest);
	Request->SetVerb("GET");
	//Request->SetURL("https://hub.dummyapis.com/delay?seconds=1");
	Request->SetURL("https://postman-echo.com/get?foo1=bar1&foo2=bar2");
	Request->ProcessRequest();
}

void UWebApiBlueprintAsyncActionBase::HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess)
{
	if (bSuccess && HttpResponse.IsValid())
	{
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogTemp, Display, TEXT("Web API Response: %s"), *ResponseString);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			FString Result;
			JsonObject->TryGetStringField("result", Result);
			OnSuccess.Broadcast(Result);
			return;
		}
	}

	OnFailure.Broadcast("");
}
