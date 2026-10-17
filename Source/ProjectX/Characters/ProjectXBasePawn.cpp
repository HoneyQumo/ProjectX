// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBasePawn.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AProjectXBasePawn::AProjectXBasePawn()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetSphereRadius(50.f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CollisionComponent;
	
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement component"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
}

// Called to bind functionality to input
void AProjectXBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectXBasePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectXBasePawn::MoveRight);
}

void AProjectXBasePawn::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	} 
}

void AProjectXBasePawn::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	} 
}

