#include "LedgeDetectorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ProjectX/ProjectXTypes.h"
#include "ProjectX/Utils/PXTraceUtils.h"


void ULedgeDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("ULedgeDetectorComponent::BeginPlay() only a character can use ULedgeDetectorComponent"))
	CachedCharacterOwner = StaticCast<ACharacter*>(GetOwner());
}

bool ULedgeDetectorComponent::DetectLedge(FLedgeDescription& LedgeDescription)
{
	UCapsuleComponent* CapsuleComponent = CachedCharacterOwner->GetCapsuleComponent();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

	float BottomZOffset = 2.f;
	FVector CharacterBottom = CachedCharacterOwner->GetActorLocation() - (CapsuleComponent->GetScaledCapsuleHalfHeight() - BottomZOffset) * FVector::UpVector;

	// 1. Forward check
	float ForwardCheckCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5f;

	FHitResult ForwardCheckHitResult;
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	float DrawTime = 2.f;

	if (!PXTraceUtils::SweepCapsuleSingleByChannel(
		GetWorld(), ForwardCheckHitResult, ForwardStartLocation,
		ForwardEndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight,
		FQuat::Identity, ECC_Climbing, QueryParams,
		FCollisionResponseParams::DefaultResponseParam, true, DrawTime
	))
	{
		return false;
	}

	// 2. Downward check
	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float DownwardCheckDepthOffset = 10.f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	if (!PXTraceUtils::SweepSphereSingleByChannel(
		GetWorld(), DownwardCheckHitResult,
		DownwardStartLocation, DownwardEndLocation,
		DownwardSphereCheckRadius, ECC_Climbing,
		QueryParams, FCollisionResponseParams::DefaultResponseParam,
		true, DrawTime
	))
	{
		return false;
	}


	// 3. Overlap check
	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	float OverlapCapsuleFloorOffset = 2.f;
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + (OverlapCapsuleHalfHeight + OverlapCapsuleFloorOffset) * FVector::UpVector;

	if (PXTraceUtils::OverlapCapsuleAnyByProfile(
		GetWorld(), OverlapLocation, OverlapCapsuleRadius, OverlapCapsuleHalfHeight,
		FQuat::Identity, FName("Pawn"), QueryParams, true, DrawTime
	))
	{
		return false;
	}


	LedgeDescription.Location = DownwardCheckHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.f, -1.f, 0.f)).ToOrientationRotator();

	return true;
}
