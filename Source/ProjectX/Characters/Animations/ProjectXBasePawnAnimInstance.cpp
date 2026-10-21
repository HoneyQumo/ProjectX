// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBasePawnAnimInstance.h"
#include "../ProjectXBasePawn.h"
#include "../../Components//MovementComponents/ProjectXBasePawnMovementComponent.h"

void UProjectXBasePawnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<AProjectXBasePawn>(),
	       TEXT("UProjectXBasePawnAnimInstance::NativeBeginPlay only AProjectXBasePawn can work with UProjectXBasePawnAnimInstance"));
	CachedBasePawn = StaticCast<AProjectXBasePawn*>(TryGetPawnOwner());
}

void UProjectXBasePawnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedBasePawn.IsValid())
	{
		return;
	}

	InputForward = CachedBasePawn->GetInputForward();
	InputRight = CachedBasePawn->GetInputRight();
	bIsInAir = CachedBasePawn->GetMovementComponent()->IsFalling();
}
