// Fill out your copyright notice in the Description page of Project Settings.

#include "ListenBlueprintAsyncActionBase.h"

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

struct FConfigData
{
	FString STTSubscriptionKey;
	FString STTRegion;
};

UListenBlueprintAsyncActionBase* UListenBlueprintAsyncActionBase::SampleListen()
{
	FString ConfigPath = FPaths::ProjectSavedDir() + TEXT("Config/Sample.auth");
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *ConfigPath);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	FConfigData ConfigData;
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		JsonObject->TryGetStringField("STTSubscriptionKey", ConfigData.STTSubscriptionKey);
		JsonObject->TryGetStringField("STTRegion", ConfigData.STTRegion);

		UE_LOG(LogTemp, Log, TEXT("SubscriptionKey: %s"), *ConfigData.STTSubscriptionKey);
		UE_LOG(LogTemp, Log, TEXT("Region: %s"), *ConfigData.STTRegion);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *ConfigPath);
	}

	UListenBlueprintAsyncActionBase* ListenAction = NewObject<UListenBlueprintAsyncActionBase>();
	ListenAction->SubscriptionKey = *ConfigData.STTSubscriptionKey;
	ListenAction->Region = *ConfigData.STTRegion;
	return ListenAction;
}

void UListenBlueprintAsyncActionBase::Activate()
{
	//Reference: https://github.com/Azure-Samples/cognitive-services-speech-sdk/blob/master/samples/cpp/windows/console/samples/speech_recognition_samples.cpp

	auto Config = SpeechConfig::FromSubscription(TCHAR_TO_UTF8(*SubscriptionKey), TCHAR_TO_UTF8(*Region));

	auto Recognizer = SpeechRecognizer::FromConfig(Config);
	auto recognitionResult = Recognizer->RecognizeOnceAsync().get();
	if (recognitionResult->Reason == ResultReason::RecognizedSpeech)
	{
		FString recognizedText = FString(recognitionResult->Text.c_str());
		OnSuccess.Broadcast(recognizedText);
	}
	else
	{
		OnFailure.Broadcast("Speech could not be recognized.");
	}
}