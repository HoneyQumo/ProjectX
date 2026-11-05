// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveVector.h"
#include "ProjectX/Actors/Interactive/InteractiveActor.h"
#include "ProjectX/Actors/Interactive/Environment/Ladder.h"
#include "../Components/LedgeDetectorComponent.h"
#include "../Components/MovementComponents/PXBaseCharacterMovementComponent.h"

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

void AProjectXBaseCharacter::ClimbLadderUp(float Value)
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder() && !FMath::IsNearlyZero(Value))
	{
		FVector LadderUpVector = GetBaseCharacterMovementComponent()->GetCurrentLadder()->GetActorUpVector();
		AddMovementInput(LadderUpVector, Value);
	}
}

void AProjectXBaseCharacter::InteractWithLadder()
{
	if (GetBaseCharacterMovementComponent()->IsOnLadder())
	{
		GetBaseCharacterMovementComponent()->DetachFromLadder();
	}
	else
	{
		const ALadder* AvailableLadder = GetAvailableLadder();
		if (IsValid(AvailableLadder))
		{
			GetBaseCharacterMovementComponent()->AttachToLadder(AvailableLadder);
		}
	}
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
		MantlingParameters.InitialLocation = GetActorLocation();
		MantlingParameters.InitialRotation = GetActorRotation();
		MantlingParameters.TargetLocation = LedgeDescription.Location;
		MantlingParameters.TargetRotation = LedgeDescription.Rotation;

		float MantlingHeight = (MantlingParameters.TargetLocation - MantlingParameters.InitialLocation).Z;
		const FMantlingSettings& MantlingSettings = GetMantlingSettings(MantlingHeight);

		float MinRange;
		float MaxRange;
		MantlingSettings.MantlingCurve->GetTimeRange(MinRange, MaxRange);
		MantlingParameters.Duration = MaxRange - MinRange;


		// float StartTime = MantlingSettings.MaxHeightStartTime - (MantlingHeight - MantlingSettings.MinHeight) / (MantlingSettings.MaxHeight - MantlingSettings.MinHeight) * (MantlingSettings.MaxHeightStartTime - MantlingSettings.MinHeightStartTime);
		MantlingParameters.MantlingCurve = MantlingSettings.MantlingCurve;

		FVector2D SourceRange(MantlingSettings.MinHeight, MantlingSettings.MaxHeight);
		FVector2D TargetRange(MantlingSettings.MinHeightStartTime, MantlingSettings.MaxHeightStartTime);
		MantlingParameters.StartTime = FMath::GetMappedRangeValueClamped(SourceRange, TargetRange, MantlingHeight);

		MantlingParameters.InitialAnimationLocation =
			MantlingParameters.TargetLocation - MantlingSettings.AnimationCorrectionZ
			* FVector::UpVector + MantlingSettings.AnimationCorrectionXY
			* LedgeDescription.LedgeNormal;

		GetBaseCharacterMovementComponent()->StartMantle(MantlingParameters);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(MantlingSettings.MantlingMontage, 1.f, EMontagePlayReturnType::Duration, MantlingParameters.StartTime);
	}
}

void AProjectXBaseCharacter::RegisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.Add(InteractiveActor);
}

void AProjectXBaseCharacter::UnregisterInteractiveActor(AInteractiveActor* InteractiveActor)
{
	AvailableInteractiveActors.Remove(InteractiveActor);
}

const ALadder* AProjectXBaseCharacter::GetAvailableLadder() const
{
	const ALadder* Result = nullptr;
	for (const AInteractiveActor* InteractiveActor : AvailableInteractiveActors)
	{
		if (InteractiveActor->IsA<ALadder>())
		{
			Result = StaticCast<const ALadder*>(InteractiveActor);
			break;
		}
	}
	
	return Result;
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

const FMantlingSettings& AProjectXBaseCharacter::GetMantlingSettings(float LedgeHeight) const
{
	return LedgeHeight > LowMantleMaxHeight ? HighMantleSettings : LowMantleSettings;
}
