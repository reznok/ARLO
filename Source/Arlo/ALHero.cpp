// Fill out your copyright notice in the Description page of Project Settings.


#include "ALHero.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GMCPlayerController.h"
#include "ActorComponents/ALMovementComponent.h"
#include "Iris/ReplicationState/ReplicationStateUtil.h"


// Sets default values
AALHero::AALHero()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UALMovementComponent>(TEXT("ALMovementComponent"));
}

void AALHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AALHero::OnJumpAction);
		EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Started, this, &AALHero::OnTeleportAction);

		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AALHero::OnMoveRightAction);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AALHero::OnMoveRightAction);
	}
}

// Called when the game starts or when spawned
void AALHero::BeginPlay()
{
	Super::BeginPlay();

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
	FGMCAbilityData JumpAbility;
	JumpAbility.GrantedAbilityIndex = 0;
	MovementComponent->QueueAbility(JumpAbility);
}

void AALHero::OnTeleportAction(const FInputActionValue& Value)
{
	const AGMC_PlayerController* playerController = (AGMC_PlayerController*)GetWorld()->GetFirstPlayerController();
	FHitResult OutHit;

	// Raycast to see if mouse hits the backdrop
	playerController->GetHitResultUnderCursor(ECC_WorldStatic, false, OutHit);

	if (OutHit.bBlockingHit)
	{
		FGMCAbilityData TeleportAbility;
		TeleportAbility.GrantedAbilityIndex = 1;
		TeleportAbility.TargetVector0 = FVector(OutHit.Location.X, 0, OutHit.Location.Z);
		TeleportAbility.ActionInput = TeleportAction;
		MovementComponent->QueueAbility(TeleportAbility);
	}
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

