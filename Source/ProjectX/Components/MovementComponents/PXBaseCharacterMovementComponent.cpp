// Fill out your copyright notice in the Description page of Project Settings.


#include "PXBaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

float UPXBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();

	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}

	return Result;
}

void UPXBaseCharacterMovementComponent::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}

void UPXBaseCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}

void UPXBaseCharacterMovementComponent::StartMantle(const FLedgeDescription& LedgeDescription)
{
	TargetLedge = LedgeDescription;
	SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling));
}

void UPXBaseCharacterMovementComponent::EndMantle()
{
	SetMovementMode(MOVE_Walking);
}

bool UPXBaseCharacterMovementComponent::IsMantling() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling);
}

void UPXBaseCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling):
		{
			float ProgressRatio = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) / TargetMantlingTime;
			FVector NewLocation = FMath::Lerp(InitialMantlingLocation, TargetLedge.Location, ProgressRatio);
			FRotator NewRotation = FMath::Lerp(InitialMantlingRotation, TargetLedge.Rotation, ProgressRatio);

			FVector Delta = NewLocation - GetActorLocation();

			FHitResult HitResult;
			SafeMoveUpdatedComponent(Delta, NewRotation, false, HitResult);

			break;
		}

	default:
		break;
	}

	Super::PhysCustom(deltaTime, Iterations);
}

void UPXBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Swimming)
	{
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(
			DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(),
			DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(),
			true
		);
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling):
			{
				InitialMantlingLocation = GetActorLocation();
				InitialMantlingRotation = GetOwner()->GetActorRotation();
				TargetMantlingTime = 0.25f;
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UPXBaseCharacterMovementComponent::EndMantle, TargetMantlingTime, false);
				break;
			}


		default:
			break;
		}
	}
}
