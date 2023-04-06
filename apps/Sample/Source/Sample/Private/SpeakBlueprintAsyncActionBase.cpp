// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeakBlueprintAsyncActionBase.h"

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

USpeakBlueprintAsyncActionBase* USpeakBlueprintAsyncActionBase::SampleSpeak(const FString& SubscriptionKey, const FString& Region, const FString& Text)
{
	USpeakBlueprintAsyncActionBase* SpeakAction = NewObject<USpeakBlueprintAsyncActionBase>();
	SpeakAction->SubscriptionKey = SubscriptionKey;
	SpeakAction->Region = Region;
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