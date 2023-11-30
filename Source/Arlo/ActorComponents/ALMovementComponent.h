#pragma once

#include "Components/GMCAbilityComponent.h"
#include "ALMovementComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UALAttributeSet: public UGMCAttributeSet
{
	GENERATED_BODY()
	
public:
	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	FAttribute Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	FAttribute MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	FAttribute Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	FAttribute MaxStamina;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARLO_API UALMovementComponent : public UGMC_AbilityComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D MovementInput;
	
	UPROPERTY(BlueprintReadWrite)
	bool bWantsToJump;

	UPROPERTY(EditAnywhere)
	float MaxFallSpeed = -1800.f;
	
	UPROPERTY(EditAnywhere)
	float MaxHorizontalMoveSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpForce = 1000.f;

	UPROPERTY(EditAnywhere)
	float Gravity = -1700.f;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsGrounded;
	
	// How many extra jumps the character has. This doesn't include jumping from a surface.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ExtraJumpCharges = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ExtraJumpChargesRemaining = 0;

	UFUNCTION(BlueprintCallable)
	bool CanJump();

private:
	void CheckIsGrounded();
	void ApplyGravity(float DeltaTime);
	void ApplyMovementInputs();

	virtual void BeginPlay() override;

protected:
	virtual void BindReplicationData_Implementation() override;
	virtual void GenPredictionTick_Implementation(float DeltaTime) override;
	
};

