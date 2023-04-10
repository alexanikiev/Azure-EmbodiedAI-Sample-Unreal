// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeakBlueprintAsyncActionBase.h"

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

struct FConfigData
{
	FString TTSSubscriptionKey;
	FString TTSRegion;
};

USpeakBlueprintAsyncActionBase* USpeakBlueprintAsyncActionBase::SampleSpeak(const FString& Text)
{
	FString ConfigPath = FPaths::ProjectSavedDir() + TEXT("Config/Sample.auth");
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *ConfigPath);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
	FConfigData ConfigData;
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		JsonObject->TryGetStringField("TTSSubscriptionKey", ConfigData.TTSSubscriptionKey);
		JsonObject->TryGetStringField("TTSRegion", ConfigData.TTSRegion);

		UE_LOG(LogTemp, Log, TEXT("SubscriptionKey: %s"), *ConfigData.TTSSubscriptionKey);
		UE_LOG(LogTemp, Log, TEXT("Region: %s"), *ConfigData.TTSRegion);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *ConfigPath);
	}

	USpeakBlueprintAsyncActionBase* SpeakAction = NewObject<USpeakBlueprintAsyncActionBase>();
	SpeakAction->SubscriptionKey = *ConfigData.TTSSubscriptionKey;
	SpeakAction->Region = *ConfigData.TTSRegion;
	SpeakAction->Text = Text;
	return SpeakAction;
}

void USpeakBlueprintAsyncActionBase::Activate()
{
	//Reference: https://github.com/Azure-Samples/cognitive-services-speech-sdk/blob/master/samples/cpp/windows/console/samples/speech_synthesis_samples.cpp

	auto Config = SpeechConfig::FromSubscription(TCHAR_TO_UTF8(*SubscriptionKey), TCHAR_TO_UTF8(*Region));
	Config->SetSpeechSynthesisVoiceName("en-US-AriaNeural");

	auto Synthesizer = SpeechSynthesizer::FromConfig(Config);
	auto synthesisResult = Synthesizer->SpeakTextAsync(TCHAR_TO_UTF8(*Text)).get();
	if (synthesisResult->Reason == ResultReason::SynthesizingAudioCompleted)
	{
		OnSuccess.Broadcast(TCHAR_TO_UTF8(*Text));
	}
	else
	{
		OnFailure.Broadcast("Speech could not be synthesized.");
	}
}