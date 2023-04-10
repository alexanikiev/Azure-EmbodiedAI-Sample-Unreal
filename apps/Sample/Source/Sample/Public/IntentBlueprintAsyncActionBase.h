// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "IntentBlueprintAsyncActionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntentResult, const FString&, Result);

/**
 * Placeholder for intent recognition which can be done by OpenAI or Azure Cognitive Services (CLU/LUIS)
 */
UCLASS()
class SAMPLE_API UIntentBlueprintAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sample")
		static UIntentBlueprintAsyncActionBase* SampleIntent(const FString& Text);

	UPROPERTY(BlueprintAssignable)
		FOnIntentResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnIntentResult OnFailure;

	virtual void Activate() override;

private:
	FString Text;
};
