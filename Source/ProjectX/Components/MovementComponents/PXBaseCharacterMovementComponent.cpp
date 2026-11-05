// Fill out your copyright notice in the Description page of Project Settings.


#include "PXBaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "../../Actors/Interactive/Environment/Ladder.h"
#include "../../Characters/ProjectXBaseCharacter.h"

void UPXBaseCharacterMovementComponent::PhysicsRotation(float DeltaTime)
{
	if (IsOnLadder())
	{
		return;
	}

	Super::PhysicsRotation(DeltaTime);
}

float UPXBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();

	if (bIsSprinting)
	{
		Result = SprintSpeed;
	}
	else if (IsOnLadder())
	{
		Result = ClimbingOnLadderMaxSpeed;
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

void UPXBaseCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingParameters)
{
	CurrentMantlingParameters = MantlingParameters;
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

float UPXBaseCharacterMovementComponent::GetActorToCurrentLadderProjection(const FVector& Location) const
{
	checkf(IsValid(CurrentLadder), TEXT("UPXBaseCharacterMovementComponent::GetActorToCurrentLadderProjection() can't ba invoked when current ladder is null"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderToCharacterDistance = Location - CurrentLadder->GetActorLocation();
	return FVector::DotProduct(LadderUpVector, LadderToCharacterDistance);
}

float UPXBaseCharacterMovementComponent::GetLadderSpeedRatio() const
{
	checkf(IsValid(CurrentLadder), TEXT("UPXBaseCharacterMovementComponent::GetLadderSpeedRatio() can't ba invoked when current ladder is null"))

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	return FVector::DotProduct(LadderUpVector, Velocity) / ClimbingOnLadderMaxSpeed;
}

void UPXBaseCharacterMovementComponent::AttachToLadder(const ALadder* Ladder)
{
	CurrentLadder = Ladder;

	FRotator TargetOrientationRotation = CurrentLadder->GetActorForwardVector().ToOrientationRotator();
	TargetOrientationRotation.Yaw += 180.f;

	FVector LadderUpVector = CurrentLadder->GetActorUpVector();
	FVector LadderForwardVector = CurrentLadder->GetActorForwardVector();
	float Projection = GetActorToCurrentLadderProjection(GetActorLocation());

	FVector NewCharacterLocation = CurrentLadder->GetActorLocation() + Projection * LadderUpVector + LadderToCharacterOffset * LadderForwardVector;

	GetOwner()->SetActorLocation(NewCharacterLocation);
	GetOwner()->SetActorRotation(TargetOrientationRotation);

	SetMovementMode(MOVE_Custom, static_cast<uint8>(ECustomMovementMode::CMOVE_Ladder));
}

void UPXBaseCharacterMovementComponent::DetachFromLadder()
{
	SetMovementMode(MOVE_Falling);
}

bool UPXBaseCharacterMovementComponent::IsOnLadder() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Ladder);
}

const ALadder* UPXBaseCharacterMovementComponent::GetCurrentLadder() const
{
	return CurrentLadder;
}

void UPXBaseCharacterMovementComponent::PhysMantling(float deltaTime, int32 Iterations)
{
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) + CurrentMantlingParameters.StartTime;

	FVector MantlingCurveValue = CurrentMantlingParameters.MantlingCurve->GetVectorValue(ElapsedTime);

	float PositionAlpha = MantlingCurveValue.X;
	float XYCorrectionAlpha = MantlingCurveValue.Y;
	float ZCorrectionAlpha = MantlingCurveValue.Z;

	FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingParameters.InitialLocation, CurrentMantlingParameters.InitialAnimationLocation,
	                                               XYCorrectionAlpha);
	CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingParameters.InitialLocation.Z, CurrentMantlingParameters.InitialAnimationLocation.Z, ZCorrectionAlpha);

	FVector NewLocation = FMath::Lerp(CorrectedInitialLocation, CurrentMantlingParameters.TargetLocation, PositionAlpha);
	FRotator NewRotation = FMath::Lerp(CurrentMantlingParameters.InitialRotation, CurrentMantlingParameters.TargetRotation, PositionAlpha);

	FVector Delta = NewLocation - GetActorLocation();

	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, NewRotation, false, HitResult);
}

void UPXBaseCharacterMovementComponent::PhysLadder(float deltaTime, int32 Iterations)
{
	CalcVelocity(deltaTime, 1.f, false, ClimbingOnLadderBrakingDeceleration);
	FVector Delta = Velocity * deltaTime;

	FVector NewPos = GetActorLocation() + Delta;
	float NewPosProjection = GetActorToCurrentLadderProjection(NewPos);

	if (NewPosProjection < MinLadderBottomOffset)
	{
		DetachFromLadder();
		return;
	}
	if (NewPosProjection > (CurrentLadder->GetLadderHeight() - MaxLadderTopOffset))
	{
		GetBaseCharacterOwner()->Mantle();
		return;
	}

	FHitResult HitResult;
	SafeMoveUpdatedComponent(Delta, GetOwner()->GetActorRotation(), true, HitResult);
}


void UPXBaseCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling):
		PhysMantling(deltaTime, Iterations);
		break;

	case static_cast<uint8>(ECustomMovementMode::CMOVE_Ladder):
		PhysLadder(deltaTime, Iterations);
		break;

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

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == static_cast<uint8>(ECustomMovementMode::CMOVE_Ladder))
	{
		CurrentLadder = nullptr;
	}

	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case static_cast<uint8>(ECustomMovementMode::CMOVE_Mantling):
			{
				GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UPXBaseCharacterMovementComponent::EndMantle, CurrentMantlingParameters.Duration, false);
				break;
			}


		default:
			break;
		}
	}
}

AProjectXBaseCharacter* UPXBaseCharacterMovementComponent::GetBaseCharacterOwner() const
{
	return StaticCast<AProjectXBaseCharacter*>(CharacterOwner);
}
