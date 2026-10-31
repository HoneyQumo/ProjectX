#pragma once

#include "CoreMinimal.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"

class UWorld;
struct FHitResult;

namespace PXTraceUtils
{
	bool SweepCapsuleSingleByChannel(
		const UWorld* World, struct FHitResult& OutHit,
		const FVector& Start, const FVector& End,
		float CapsuleRadius, float CapsuleHalfHeight,
		const FQuat& Rot, ECollisionChannel TraceChannel,
		const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam,
		bool bDrawDebug = false, float DrawTime = -1.f,
		FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red
	);

	bool SweepSphereSingleByChannel(
		const UWorld* World, struct FHitResult& OutHit,
		const FVector& Start, const FVector& End,
		float Radius, ECollisionChannel TraceChannel,
		const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam,
		bool bDrawDebug = false, float DrawTime = -1.f,
		FColor TraceColor = FColor::Black, FColor HitColor = FColor::Red
	);

	bool OverlapCapsuleAnyByProfile(
		const UWorld* World, const FVector& Pos,
		float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rotation,
		FName ProfileName, const FCollisionQueryParams& QueryParams,
		bool bDrawDebug = false, float DrawTime = -1.f,
		FColor HitColor = FColor::Red
	);
}
