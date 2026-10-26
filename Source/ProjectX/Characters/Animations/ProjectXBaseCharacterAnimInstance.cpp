// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBaseCharacterAnimInstance.h"
#include "../ProjectXBaseCharacter.h"
#include "../../Components/MovementComponents/PXBaseCharacterMovementComponent.h"

void UProjectXBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<AProjectXBaseCharacter>(),
	       TEXT("UProjectXBaseCharacterAnimInstance::NativeBeginPlay(): UProjectXBaseCharacterAnimInstance can be used only with AProjectXBaseCharacter"));
	CachedBaseCharacter = StaticCast<AProjectXBaseCharacter*>(TryGetPawnOwner());
}

void UProjectXBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedBaseCharacter.IsValid())
	{
		return;
	}

	UPXBaseCharacterMovementComponent* CharacterMovementComponent = CachedBaseCharacter->GetBaseCharacterMovementComponent();
	Speed = CharacterMovementComponent->Velocity.Size2D();
	bIsFalling = CharacterMovementComponent->IsFalling();
	bIsCrouching = CharacterMovementComponent->IsCrouching();
	bIsSprinting = CharacterMovementComponent->IsSprinting();
}
