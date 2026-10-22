// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBasePawn.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectX/ProjectX.h"
#include "ProjectX/Components/MovementComponents/ProjectXBasePawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectXBasePawn, Log, All);

// Sets default values
AProjectXBasePawn::AProjectXBasePawn()
{
	bUseControllerRotationYaw = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComponent->SetSphereRadius(CollisionSphereRadius);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UProjectXBasePawnMovementComponent>(TEXT("Movement component"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);

#if	WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
#endif

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->TargetArmLength = 500.f;
	SpringArmComponent->bUsePawnControlRotation = 1;
	SpringArmComponent->SetupAttachment(RootComponent);

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetAutoActivate(false);
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
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AProjectXBasePawn::SwitchCamera);
}

void AProjectXBasePawn::MoveForward(float Value)
{
	InputForward = Value;
	if (Value != 0.f)
	{
		AddMovementInput(CurrentViewActor->GetActorForwardVector(), Value);
	}
}

void AProjectXBasePawn::MoveRight(float Value)
{
	InputRight = Value;
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
	CurrentViewActor = GetController()->GetViewTarget();
	UE_LOG(LogCameras, Log, TEXT("AProjectXBasePawn::OnBlendComplete() Current view actor: %s"), *CurrentViewActor->GetName());
}

void AProjectXBasePawn::SwitchCamera()
{
	const bool bIsFirstPerson = FirstPersonCamera->IsActive();
	FirstPersonCamera->SetActive(!bIsFirstPerson);
	ThirdPersonCamera->SetActive(bIsFirstPerson);

	UE_LOG(LogProjectXBasePawn, Log, TEXT("AProjectXBasePawn::SwitchCamera() turn on: %s"), bIsFirstPerson ? TEXT("ThirdPersonCamera") : TEXT("FirstPersonCamera"));
}
