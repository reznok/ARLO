// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UIPing.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARLO_API UUIPing : public UUserWidget
{
	GENERATED_BODY()
	// Doing setup in the C++ constructor is not as
	// useful as using NativeConstruct.
	virtual void NativeConstruct() override;
//
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI")
	UTextBlock* PingText = nullptr;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetPingText(FString Ping);

};
