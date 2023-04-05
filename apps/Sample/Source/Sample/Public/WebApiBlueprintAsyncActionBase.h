// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"
#include "WebApiBlueprintAsyncActionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebApiResult, const FString&, Result);

/**
 * Functionality to integrate with custom or first-party Web APIs (Azure OpenAI ChatGPT, CLU/LUIS, Azure Cognitive Search, etc.)
 */
UCLASS()
class SAMPLE_API UWebApiBlueprintAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Web API")
		static UWebApiBlueprintAsyncActionBase* SampleWebApi();

	UPROPERTY(BlueprintAssignable)
		FWebApiResult OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FWebApiResult OnFailure;

	virtual void Activate() override;

private:
	void HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSuccess);
};
