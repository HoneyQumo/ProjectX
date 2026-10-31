// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Components/LedgeDetectorComponent.h"
#include "PXBaseCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	CMOVE_None = 0 UMETA(DisplayName = "None"),
	CMOVE_Mantling UMETA(DisplayName = "Mantling"),
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

	void StartMantle(const FLedgeDescription& LedgeDescription);
	void EndMantle();
	bool IsMantling() const;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character movement: sprint", meta = (ClampMin = 0.f, UIMin = 0.f))
	float SprintSpeed = 1200.f;

	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float SwimmingCapsuleRadius = 60.f;

	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float SwimmingCapsuleHalfHeight = 60.f;

private:
	bool bIsSprinting;

	FLedgeDescription TargetLedge;
	float TargetMantlingTime = 1.0f;
	FVector InitialMantlingLocation;
	FRotator InitialMantlingRotation;

	FTimerHandle MantlingTimer;
};
