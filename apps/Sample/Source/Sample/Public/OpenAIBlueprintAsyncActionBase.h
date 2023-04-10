// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"
#include "OpenAIBlueprintAsyncActionBase.generated.h"

UENUM(BlueprintType)
enum EOpenAIApiType
{
	ChatGPT UMETA(DisplayName = "ChatGPT"),
	DALLE UMETA(DisplayName = "DALLE"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOpenAIResult, const FString&, Result);

/**
 * Functionality to integrate with OpenAI APIs (ChatGPT, DALLE, etc.)
 */
UCLASS()
class SAMPLE_API UOpenAIBlueprintAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sample")
		static UOpenAIBlueprintAsyncActionBase* SampleOpenAI(const EOpenAIApiType& OpenAIApiType = EOpenAIApiType::ChatGPT);

	UPROPERTY(BlueprintAssignable)
		FOpenAIResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FOpenAIResult OnFailure;

	virtual void Activate() override;

private:
	FString OpenAIApiKey;
	FString AzureOpenAISubscriptionKey;
	FString AzureOpenAIRegion;
	EOpenAIApiType OpenAIApiType;
	void HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess);
	FString ComposePrompt(const TArray<FString>& Inputs);
};
