// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBasePawn.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectX/ProjectX.h"
#include "ProjectX/Components/MovementComponents/ProjectXBasePawnMovementComponent.h"

// Sets default values
AProjectXBasePawn::AProjectXBasePawn()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetSphereRadius(50.f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = CollisionComponent;

	// MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("Movement component"));
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UProjectXBasePawnMovementComponent>(TEXT("Movement component"));
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjectXBasePawn::Jump);
}

void AProjectXBasePawn::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(CurrentViewActor->GetActorForwardVector(), Value);
	}
}

void AProjectXBasePawn::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(CurrentViewActor->GetActorRightVector(), Value);
	}
}

void AProjectXBasePawn::Jump()
{
	checkf(MovementComponent->IsA<UProjectXBasePawnMovementComponent>(), TEXT("AProjectXBasePawn::Jump can work only with UProjectXBasePawnMovementComponent"))
	UProjectXBasePawnMovementComponent* BaseMovement = StaticCast<UProjectXBasePawnMovementComponent*>(MovementComponent);
	BaseMovement->JumpStart();
}

void AProjectXBasePawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CurrentViewActor = CameraManager->GetViewTarget();
	CameraManager->OnBlendComplete().AddUFunction(this, FName("OnBlendComplete"));
}

void AProjectXBasePawn::OnBlendComplete()
{
	UE_LOG(LogCameras, Log, TEXT("AProjectXBasePawn::OnBlendComplete() Current view actor: %s"), *CurrentViewActor->GetName())
	CurrentViewActor = GetController()->GetViewTarget();
}
