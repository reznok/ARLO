// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Widgets/SCompoundWidget.h"
#include "UIAttributes.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARLO_API UUIAttributes : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UProgressBar* ProgressBarHealth = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UProgressBar* ProgressBarStamina = nullptr;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetHealth(float Current, float Max);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetStamina(float Current, float Max);
	
};
