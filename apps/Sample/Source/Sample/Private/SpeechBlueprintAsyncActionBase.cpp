// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechBlueprintAsyncActionBase.h"

using namespace Microsoft::CognitiveServices::Speech;

USpeechBlueprintAsyncActionBase* USpeechBlueprintAsyncActionBase::SampleSpeak()
{
	USpeechBlueprintAsyncActionBase* SpeechAction = NewObject<USpeechBlueprintAsyncActionBase>();
	return SpeechAction;
}

void USpeechBlueprintAsyncActionBase::Activate()
{
	auto Config = SpeechConfig::FromSubscription("<subscription key>", "<region>");
	Config->SetSpeechSynthesisVoiceName("en-US-AriaNeural");
	auto Synthesizer = SpeechSynthesizer::FromConfig(Config);
	Synthesizer->SpeakTextAsync("Hello");
}