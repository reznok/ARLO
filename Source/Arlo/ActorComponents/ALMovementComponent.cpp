#include "ALMovementComponent.h"


void UALMovementComponent::CheckIsGrounded()
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	// Fire a few line traces down from the player's feet to see if it collides with anything
	for (int x = 0; x < 3; x++)
	{
		FHitResult GroundHit;
		// Hard coded traces. Should be improved upon.
		FVector TraceStart = GetActorLocation()+ FVector(-20 + x*20, 0, -70);
		FVector TraceEnd = GetActorLocation() + FVector(-20 + x*20, 0, -78);
		GetWorld()->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);

		// Uncomment this to see the traces
		// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255, 0, 0), false, -1, 0,6);

		// If any of them result in a valid hit, it's grounded and can be returned out early
		if (GroundHit.bBlockingHit && IsValid(GroundHit.GetActor()))
		{
			bIsGrounded = true;
			ExtraJumpChargesRemaining = ExtraJumpCharges;
			return;
		}
	}
	bIsGrounded = false;
}

void UALMovementComponent::ApplyGravity(float DeltaTime)
{
	if (bIsGrounded)
	{
		Velocity.Z = 0.f;
	}
	else{
		// Add gravity acceleration
		// Need DeltaTime as it's a rate of change
		UpdateVelocity(FVector(0, 0, Velocity.Z + Gravity * DeltaTime));
	}

	// Clamp fall speed
	Velocity = FVector(Velocity.X, Velocity.Y, FMath::Clamp(Velocity.Z, MaxFallSpeed, INFINITY));
}

void UALMovementComponent::ApplyMovementInputs()
{	
	// Quickly stop X movement if no X input
	// Ideally this would be a deceleration to be smoother, but good for now
	if (MovementInput.X == 0)
	{
		UpdateVelocity(FVector(0.f, Velocity.Y, Velocity.Z));
	}
	else
	{
		// Add input acceleration
		UpdateVelocity(FVector(MovementInput.X * MaxHorizontalMoveSpeed, Velocity.Y, Velocity.Z));
	}
	
	// Clamp Horizontal velocity
	Velocity = FVector(FMath::Clamp(Velocity.X, -MaxHorizontalMoveSpeed, MaxHorizontalMoveSpeed), Velocity.Y, Velocity.Z);
	
}

void UALMovementComponent::BindReplicationData_Implementation()
{
	Super::BindReplicationData_Implementation();

	// Player input
	BindCompressedVector2D( MovementInput,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);
	
	BindBool( bWantsToJump,
		EGMC_PredictionMode::ClientAuth_Input,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);

	// Server Auth
	BindBool( bIsGrounded,
		EGMC_PredictionMode::ServerAuth_Output_ClientValidated,
		EGMC_CombineMode::CombineIfUnchanged,
		EGMC_SimulationMode::Periodic_Output,
		EGMC_InterpolationFunction::Linear);
}

void UALMovementComponent::GenPredictionTick_Implementation(float DeltaTime)
{
	Super::GenPredictionTick_Implementation(DeltaTime);
	
	// Store original location for computing new velocity (used for slides, bonks, etc.)
	const FVector OldLocation = GetOwner()->GetActorLocation();
	
	CheckIsGrounded();
	ApplyGravity(DeltaTime);
	ApplyMovementInputs();
	
	if (bWantsToJump)
	{
		Jump();
		bWantsToJump = false;
	}

	// Move Actor
	const FVector NewLocation = GetOwner()->GetActorLocation() + Velocity * DeltaTime;
	FHitResult OutHit(1.f);
	GetOwner()->SetActorLocation(NewLocation, true, &OutHit, ETeleportType::None);
	
	if (OutHit.bBlockingHit)
	{
		FVector SlidingVector = ComputeSlideVector(Velocity * DeltaTime,(1.f - OutHit.Time), OutHit.Normal, OutHit);
		SlidingVector.Y = 0;
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + SlidingVector, false, &OutHit, ETeleportType::None);
	}
	
	// Calculate Velocity based off movement that was applied
	// Without this, things like bonking your head into a platform would be very floaty as gravity
	// slowly pulls down the velocity.
	Velocity = (GetOwner()->GetActorLocation() - OldLocation) / DeltaTime;
}

void UALMovementComponent::Jump()
{
	if (bIsGrounded || ExtraJumpChargesRemaining > 0)
	{
		// Overwrite vertical velocity with jump force
		UpdateVelocity(FVector(Velocity.X, 0, JumpForce));

		// Double jumping
		if (!bIsGrounded && ExtraJumpChargesRemaining > 0)
		{
			ExtraJumpChargesRemaining -= 1;
		}
	}
}
