#include "PXTraceUtils.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

namespace PXTraceUtils
{
	bool SweepCapsuleSingleByChannel(
		const UWorld* World, struct FHitResult& OutHit,
		const FVector& Start, const FVector& End,
		float CapsuleRadius, float CapsuleHalfHeight,
		const FQuat& Rot, ECollisionChannel TraceChannel,
		const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParam,
		bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor
	)
	{
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
		const bool bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
		if (bDrawDebug)
		{
			DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, Rot, TraceColor, false, DrawTime);
			DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, Rot, TraceColor, false, DrawTime);
			DrawDebugLine(World, Start, End, TraceColor, false, DrawTime);

			if (bResult)
			{
				DrawDebugCapsule(World, OutHit.Location, CapsuleHalfHeight, CapsuleRadius, Rot, HitColor, false, DrawTime);
				DrawDebugPoint(World, OutHit.ImpactPoint, 10.f, HitColor, false, DrawTime);
			}
		}
#endif

		return bResult;
	}

	bool SweepSphereSingleByChannel(
		const UWorld* World, struct FHitResult& OutHit,
		const FVector& Start, const FVector& End, float Radius,
		ECollisionChannel TraceChannel, const FCollisionQueryParams& Params,
		const FCollisionResponseParams& ResponseParam, bool bDrawDebug, float DrawTime,
		FColor TraceColor, FColor HitColor
	)
	{
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
		const bool bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
		if (bDrawDebug)
		{
			FVector DebugCenter = (Start + End) * 0.5f;
			FVector TraceVector = End - Start;
			float DebugCapsuleHalfHeight = TraceVector.Size() * 0.5f;

			FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

			DrawDebugCapsule(World, DebugCenter, DebugCapsuleHalfHeight, Radius, DebugCapsuleRotation, TraceColor, false, DrawTime);

			if (bResult)
			{
				DrawDebugSphere(World, OutHit.Location, Radius, 32, HitColor, false, DrawTime);
				DrawDebugPoint(World, OutHit.ImpactPoint, 10.f, HitColor, false, DrawTime);
			}
		}
#endif

		return bResult;
	}

	bool OverlapCapsuleAnyByProfile(
		const UWorld* World, const FVector& Pos, float CapsuleRadius,
		float CapsuleHalfHeight, const FQuat& Rotation, FName ProfileName,
		const FCollisionQueryParams& QueryParams,
		bool bDrawDebug, float DrawTime, FColor HitColor
	)
	{
		FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
		const bool bResult = World->OverlapAnyTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);

#if ENABLE_DRAW_DEBUG
		if (bDrawDebug && bResult)
		{
			DrawDebugCapsule(
				World, Pos, CapsuleHalfHeight, CapsuleRadius,
				Rotation,
				HitColor,
				false, DrawTime
			);
		}
#endif

		return bResult;
	}
}
