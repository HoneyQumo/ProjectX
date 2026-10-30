// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXPlayerController.h"
#include "../ProjectXBaseCharacter.h"

void AProjectXPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	CachedBaseCharacter = Cast<AProjectXBaseCharacter>(InPawn);
}

void AProjectXPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AProjectXPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AProjectXPlayerController::MoveRight);
	InputComponent->BindAxis("Turn", this, &AProjectXPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &AProjectXPlayerController::LookUp);
	InputComponent->BindAxis("SwimForward", this, &AProjectXPlayerController::SwimForward);
	InputComponent->BindAxis("SwimRight", this, &AProjectXPlayerController::SwimRight);
	InputComponent->BindAxis("SwimUp", this, &AProjectXPlayerController::SwimUp);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AProjectXPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AProjectXPlayerController::Jump);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AProjectXPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AProjectXPlayerController::StopSprint);
}

void AProjectXPlayerController::MoveForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveForward(Value);
	}
}

void AProjectXPlayerController::MoveRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->MoveRight(Value);
	}
}

void AProjectXPlayerController::Turn(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Turn(Value);
	}
}

void AProjectXPlayerController::LookUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->LookUp(Value);
	}
}

void AProjectXPlayerController::ChangeCrouchState()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->ChangeCrouchState();
	}
}

void AProjectXPlayerController::Jump()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->Jump();
	}
}

void AProjectXPlayerController::StartSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StartSprint();
	}
}

void AProjectXPlayerController::StopSprint()
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->StopSprint();
	}
}

void AProjectXPlayerController::SwimForward(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimForward(Value);
	}
}

void AProjectXPlayerController::SwimRight(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimRight(Value);
	}
}

void AProjectXPlayerController::SwimUp(float Value)
{
	if (CachedBaseCharacter.IsValid())
	{
		CachedBaseCharacter->SwimUp(Value);
	}
}
