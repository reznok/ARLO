// Fill out your copyright notice in the Description page of Project Settings.


#include "UIPing.h"

#include "GMCPlayerController.h"

void UUIPing::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUIPing::SetPingText(FString Ping)
{
	if (PingText)
	{
		PingText->SetText(FText::FromString(Ping + "ms"));
	}
}
