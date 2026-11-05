// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Components/LedgeDetectorComponent.h"
#include "PXBaseCharacterMovementComponent.generated.h"

class ALadder;

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.f;
	float StartTime = 0.f;

	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
	CMOVE_Ladder UMETA(DisplayName = "Ladder"),
	CMOVE_Max UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class PROJECTX_API UPXBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsSprinting() { return bIsSprinting; }

	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	void StartMantle(const FMantlingMovementParameters& MantlingParameters);
	void EndMantle();
	bool IsMantling() const;


	void AttachToLadder(const ALadder* Ladder);
	void DetachFromLadder();
	bool IsOnLadder() const;
	const ALadder* GetCurrentLadder() const;

protected:
	void PhysMantling(float deltaTime, int32 Iterations);
	void PhysLadder(float deltaTime, int32 Iterations);
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.f, UIMin = 0.f))
	float SprintSpeed = 1200.f;

	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float SwimmingCapsuleRadius = 60.f;

	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float SwimmingCapsuleHalfHeight = 60.f;

	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float ClimbingOnLadderMaxSpeed = 200.f;

	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float ClimbingOnLadderBrakingDeceleration = 2048.f;

	const ALadder* CurrentLadder = nullptr;

private:
	bool bIsSprinting;

	FMantlingMovementParameters CurrentMantlingParameters;

	FTimerHandle MantlingTimer;
};
