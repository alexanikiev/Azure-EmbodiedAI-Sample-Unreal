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

	UE_LOG(LogTemp, Display, TEXT("Lowercase Text: %s"), *LowercaseText);

	if (LowercaseText.Contains("chat") || LowercaseText.Contains("chad"))
	{
		OutputText = "ChatGPT";
	}
	else if (LowercaseText.Contains("dalle") || LowercaseText.Contains("dali"))
	{
		OutputText = "DALLE";
	}

	OnSuccess.Broadcast(OutputText);
}