// Fill out your copyright notice in the Description page of Project Settings.

#include "IntentBlueprintAsyncActionBase.h"

UIntentBlueprintAsyncActionBase* UIntentBlueprintAsyncActionBase::SampleIntent(const FString& Text)
{
	UIntentBlueprintAsyncActionBase* IntentAction = NewObject<UIntentBlueprintAsyncActionBase>();
	IntentAction->Text = Text;
	return IntentAction;
}

void UIntentBlueprintAsyncActionBase::Activate()
{
	FString LowercaseText = Text.ToLower();
	FString OutputText = "None";

	if (LowercaseText.Contains("chat"))
	{
		OutputText = "OpenAI_ChatGPT";
	}
	else if (LowercaseText.Contains("dalle") || LowercaseText.Contains("dali"))
	{
		OutputText = "OpenAI_DALLE";
	}

	OnSuccess.Broadcast(OutputText);
}