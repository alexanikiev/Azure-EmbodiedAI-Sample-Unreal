// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include <speechapi_cxx.h>
#include "SpeechBlueprintAsyncActionBase.generated.h"

/**
 * Functionality to integrate with Azure Speech SDK (Listen, Speak, etc.)
 */
UCLASS()
class SAMPLE_API USpeechBlueprintAsyncActionBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sample")
		static USpeechBlueprintAsyncActionBase* SampleSpeak();

	virtual void Activate() override;
};
