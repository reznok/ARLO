#pragma once
#include "GMCMovementUtilityComponent.h"
#include "ALMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARLO_API UALMovementComponent : public UGMC_MovementUtilityCmp
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D MovementInput;

	UPROPERTY(EditAnywhere)
	float MaxFallSpeed = -1800.f;
	
	UPROPERTY(EditAnywhere)
	float MaxHorizontalMoveSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpForce = 1000.f;

	UPROPERTY(EditAnywhere)
	float Gravity = -1700.f;

	UPROPERTY(BlueprintReadWrite)
	bool bWantsToJump;

	UPROPERTY()
	bool bIsGrounded;

	// How many extra jumps the character has. This doesn't include jumping from a surface.
	UPROPERTY(EditAnywhere)
	int ExtraJumpCharges = 1;
	
	UPROPERTY()
	int ExtraJumpChargesRemaining = 0;

	UFUNCTION(BlueprintCallable)
	bool CanJump();

private:
	void CheckIsGrounded();
	void ApplyGravity(float DeltaTime);
	void ApplyMovementInputs();

protected:
	virtual void BindReplicationData_Implementation() override;
	virtual void GenPredictionTick_Implementation(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void Jump();
};
