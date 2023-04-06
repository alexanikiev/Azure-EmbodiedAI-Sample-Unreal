// Fill out your copyright notice in the Description page of Project Settings.

#include "ListenBlueprintAsyncActionBase.h"

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

UListenBlueprintAsyncActionBase* UListenBlueprintAsyncActionBase::SampleListen(const FString& SubscriptionKey, const FString& Region)
{
	UListenBlueprintAsyncActionBase* ListenAction = NewObject<UListenBlueprintAsyncActionBase>();
	ListenAction->SubscriptionKey = SubscriptionKey;
	ListenAction->Region = Region;
	return ListenAction;
}

void UListenBlueprintAsyncActionBase::Activate()
{
	//Reference: https://github.com/Azure-Samples/cognitive-services-speech-sdk/blob/master/samples/cpp/windows/console/samples/speech_recognition_samples.cpp

	auto Config = SpeechConfig::FromSubscription(TCHAR_TO_UTF8(*SubscriptionKey), TCHAR_TO_UTF8(*Region));
	Config->SetSpeechSynthesisVoiceName("en-US-AriaNeural");

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