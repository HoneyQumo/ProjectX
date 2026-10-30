// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectX/Components/LedgeDetectorComponent.h"
#include "ProjectX/Components/MovementComponents/PXBaseCharacterMovementComponent.h"

AProjectXBaseCharacter::AProjectXBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UPXBaseCharacterMovementComponent>(CharacterMovementComponentName))
{
	PXBaseCharacterMovementComponent = StaticCast<UPXBaseCharacterMovementComponent*>(GetCharacterMovement());

	LedgeDetectorComponent = CreateDefaultSubobject<ULedgeDetectorComponent>(TEXT("LedgeDetector"));
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
		//todo: activate mantling	
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
