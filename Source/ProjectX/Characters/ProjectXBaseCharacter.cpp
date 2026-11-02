// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/LedgeDetectorComponent.h"
#include "../Components/MovementComponents/PXBaseCharacterMovementComponent.h"
#include "Curves/CurveVector.h"

AProjectXBaseCharacter::AProjectXBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UPXBaseCharacterMovementComponent>(CharacterMovementComponentName))
{
	PXBaseCharacterMovementComponent = StaticCast<UPXBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));
}

bool AProjectXBaseCharacter::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !GetBaseCharacterMovementComponent()->IsMantling();
}

void AProjectXBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AProjectXBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;

	if (bIsCrouched)
	{
		UnCrouch();
	}
}

void AProjectXBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

void AProjectXBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TryChangeSprintState();
}

void AProjectXBaseCharacter::Mantle()
{
	FLedgeDescription LedgeDescription;

	if (LedgeDetectorComponent->DetectLedge(LedgeDescription))
	{
		FMantlingMovementParameters MantlingParameters;
		MantlingParameters.MantlingCurve = HeightMantleSettings.MantlingCurve;
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotation = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TargetRotation = LedgeDescription.Rotation;

		float MinRange;
		float MaxRange;
		HeightMantleSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);
		MantlingParameters.Duration = MaxRange - MinRange;

		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;

		// float StartTime = HeightMantleSettings.MaxHeightStartTime - (MantlingHeight - HeightMantleSettings.MinHeight) / (HeightMantleSettings.MaxHeight - HeightMantleSettings.MinHeight) * (HeightMantleSettings.MaxHeightStartTime - HeightMantleSettings.MinHeightStartTime);
		FVector2D SourceRange(HeightMantleSettings.MinHeight, HeightMantleSettings.MaxHeight);
		FVector2D TargetRange(HeightMantleSettings.MinHeightStartTime, HeightMantleSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParameters.InitialAnimationLocation =
			MantlingParameters.TargetLocation - HeightMantleSettings.AnimationCorrectionZ
			* FVector::UpVector + HeightMantleSettings.AnimationCorrectionXY
			* LedgeDescription.LedgeNormal;

		GetBaseCharacterMovementComponent()->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(HeightMantleSettings.MantlingMontage, 1.f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
	}
}

void AProjectXBaseCharacter::OnSprintStart_Implementation()
{
}

void AProjectXBaseCharacter::OnSprintEnd_Implementation()
{
}

bool AProjectXBaseCharacter::CanSprint()
{
	return true;
}

void AProjectXBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !PXBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		PXBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}

	if (!bIsSprintRequested && PXBaseCharacterMovementComponent->IsSprinting())
	{
		PXBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}
