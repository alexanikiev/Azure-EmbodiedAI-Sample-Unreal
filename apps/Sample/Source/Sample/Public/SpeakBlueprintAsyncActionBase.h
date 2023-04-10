// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include <speechapi_cxx.h>
#include "SpeakBlueprintAsyncActionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeakResult, const FString&, Result);


/**
 * Functionality to integrate with Azure Speech SDK (SpeechSynthesizer)
 */
UCLASS()
class SAMPLE_API USpeakBlueprintAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sample")
		static USpeakBlueprintAsyncActionBase* SampleSpeak(const FString& Text);

	UPROPERTY(BlueprintAssignable)
		FOnSpeakResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnSpeakResult OnFailure;

	virtual void Activate() override;

private:
	FString SubscriptionKey;
	FString Region;
	FString Text;
};
