// Fill out your copyright notice in the Description page of Project Settings.

#include "UIAttributes.h"


void UUIAttributes::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUIAttributes::SetHealth(float Current, float Max)
{
	if (ProgressBarHealth)
	{
		ProgressBarHealth->SetPercent( Current / Max );
	}
}

void UUIAttributes::SetStamina(float Current, float Max)
{
	if (ProgressBarStamina)
	{
		ProgressBarStamina->SetPercent( Current / Max );
	}
}
