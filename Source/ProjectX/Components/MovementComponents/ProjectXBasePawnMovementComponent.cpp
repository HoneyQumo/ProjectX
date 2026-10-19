// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectXBasePawnMovementComponent.h"

void UProjectXBasePawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PendingInput = GetPendingInputVector().GetClampedToMaxSize(1.f);
	Velocity = PendingInput * MaxSpeed;
	ConsumeInputVector();

	if (bEnableGravity)
	{
		FHitResult HitResult;
		FVector StartPoint = UpdatedComponent->GetComponentLocation();
		float TraceDepth = 10.f;
		float SphereHalfRadius = UpdatedComponent->Bounds.SphereRadius / 2;
		float LineTraceLength = SphereHalfRadius + TraceDepth;
		FVector EndPoint = StartPoint - LineTraceLength * FVector::UpVector;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());

		bool bWasFalling = bIsFalling;
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereHalfRadius);
		bIsFalling = !GetWorld()->SweepSingleByChannel(HitResult, StartPoint, EndPoint, FQuat::Identity, ECC_Visibility, CollisionShape, CollisionQueryParams);

		if (bIsFalling)
		{
			VerticalVelocity += GetGravityZ() * DeltaTime * FVector::UpVector;
		}
		else if (bWasFalling && VerticalVelocity.Z < 0.f)
		{
			VerticalVelocity = FVector::ZeroVector;
		}
	}

	Velocity += VerticalVelocity;
	FVector Delta = Velocity * DeltaTime;
	if (!Delta.IsNearlyZero(1e-6f))
	{
		FQuat Rot = UpdatedComponent->GetComponentQuat();
		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rot, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			// Try to slide the remaining distance along the surface.
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}
	}

	UpdateComponentVelocity();
}

void UProjectXBasePawnMovementComponent::JumpStart()
{
	VerticalVelocity = InitialJumpVelocity * FVector::UpVector;
}
