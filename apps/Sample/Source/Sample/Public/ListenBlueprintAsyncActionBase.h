// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include <speechapi_cxx.h>
#include "ListenBlueprintAsyncActionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnListenResult, const FString&, Result);


/**
 * Functionality to integrate with Azure Speech SDK (SpeechRecognizer)
 */
UCLASS()
class SAMPLE_API UListenBlueprintAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sample")
		static UListenBlueprintAsyncActionBase* SampleListen();

	UPROPERTY(BlueprintAssignable)
		FOnListenResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOnListenResult OnFailure;

	virtual void Activate() override;

private:
	FString SubscriptionKey;
	FString Region;
};
