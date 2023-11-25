// Fill out your copyright notice in the Description page of Project Settings.


#include "ALHero.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GMCPlayerController.h"
#include "ActorComponents/ALMovementComponent.h"


// Sets default values
AALHero::AALHero()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AALHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AALHero::OnJumpAction);

		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AALHero::OnMoveRightAction);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AALHero::OnMoveRightAction);
	}
}

// Called when the game starts or when spawned
void AALHero::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent = GetComponentByClass<UALMovementComponent>();
	if (MovementComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Movement component found on Actor"));
	}

	if (AGMC_PlayerController* PlayerController = Cast<AGMC_PlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Spawn players in random locations, very hacky
	if (HasAuthority())
	{
		SetActorLocation(FVector(FMath::RandRange(-400.f, 400.f), 0, 250.f));
	}
}

void AALHero::OnJumpAction(const FInputActionValue& Value)
{
	MovementComponent->bWantsToJump = true;
}

void AALHero::OnMoveRightAction(const FInputActionValue& Value)
{
	MovementComponent->MovementInput.X = Value.Get<float>();
}

// Called every frame
void AALHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

