// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GMCPawn.h"
#include "InputAction.h"
#include "ActorComponents/ALMovementComponent.h"
#include "GameFramework/Actor.h"
#include "ALHero.generated.h"

UCLASS()
class ARLO_API AALHero : public AGMC_Pawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AALHero();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultMappingContext; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveRightAction = nullptr;

	UPROPERTY()
	UALMovementComponent* MovementComponent;

	void OnJumpAction(const FInputActionValue& Value);
	void OnMoveRightAction(const FInputActionValue& Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
