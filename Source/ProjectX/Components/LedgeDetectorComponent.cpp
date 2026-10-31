#include "LedgeDetectorComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ProjectX/ProjectXTypes.h"


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
	float ForwardCheckCapsuleHalfHeight = (MaximumLedgeHeight - MinimumLedgeHeight) * 0.5;

	FHitResult ForwardCheckHitResult;
	FCollisionShape ForwardCapsuleShape = FCollisionShape::MakeCapsule(ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight);
	FVector ForwardStartLocation = CharacterBottom + (MinimumLedgeHeight + ForwardCheckCapsuleHalfHeight) * FVector::UpVector;
	FVector ForwardEndLocation = ForwardStartLocation + CachedCharacterOwner->GetActorForwardVector() * ForwardCheckDistance;

	float DrawTime = 2.f;
	DrawDebugCapsule(GetWorld(), ForwardStartLocation, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Black, false, DrawTime);
	DrawDebugCapsule(GetWorld(), ForwardEndLocation, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Black, false, DrawTime);
	DrawDebugLine(GetWorld(), ForwardStartLocation, ForwardEndLocation, FColor::Black, false, DrawTime);

	if (!GetWorld()->SweepSingleByChannel(
			ForwardCheckHitResult, ForwardStartLocation,
			ForwardEndLocation, FQuat::Identity,
			ECC_Climbing, ForwardCapsuleShape,
			QueryParams)
	)
	{
		return false;
	}
	DrawDebugCapsule(GetWorld(), ForwardCheckHitResult.Location, ForwardCheckCapsuleHalfHeight, ForwardCheckCapsuleRadius, FQuat::Identity, FColor::Red, false, DrawTime);
	DrawDebugPoint(GetWorld(), ForwardCheckHitResult.ImpactPoint, 10.f, FColor::Red, false, DrawTime);

	// 2. Downward check
	FHitResult DownwardCheckHitResult;
	float DownwardSphereCheckRadius = CapsuleComponent->GetScaledCapsuleRadius();
	FCollisionShape DownwardSphereShape = FCollisionShape::MakeSphere(DownwardSphereCheckRadius);

	float DownwardCheckDepthOffset = 10.f;
	FVector DownwardStartLocation = ForwardCheckHitResult.ImpactPoint - ForwardCheckHitResult.ImpactNormal * DownwardCheckDepthOffset;
	DownwardStartLocation.Z = CharacterBottom.Z + MaximumLedgeHeight + DownwardSphereCheckRadius;
	FVector DownwardEndLocation(DownwardStartLocation.X, DownwardStartLocation.Y, CharacterBottom.Z);

	FVector DebugDrawCapsuleLocation = (DownwardStartLocation + DownwardEndLocation) * 0.5f;
	float DebugDrawCapsuleHalfHeight = (DownwardEndLocation - DownwardStartLocation).Size() * 0.5f;

	DrawDebugCapsule(GetWorld(), DebugDrawCapsuleLocation, DebugDrawCapsuleHalfHeight, DownwardSphereCheckRadius, FQuat::Identity, FColor::Black, false, DrawTime);

	if (!GetWorld()->SweepSingleByChannel(
			DownwardCheckHitResult, DownwardStartLocation,
			DownwardEndLocation, FQuat::Identity,
			ECC_Climbing, DownwardSphereShape,
			QueryParams)
	)
	{
		return false;
	}

	DrawDebugSphere(GetWorld(), DownwardCheckHitResult.Location, DownwardSphereCheckRadius, 32, FColor::Red, false, DrawTime);
	DrawDebugPoint(GetWorld(), DownwardCheckHitResult.ImpactPoint, 10.f, FColor::Red, false, DrawTime);


	// 3. Overlap check
	float OverlapCapsuleRadius = CapsuleComponent->GetScaledCapsuleRadius();
	float OverlapCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	FCollisionShape OverlapCapsuleShape = FCollisionShape::MakeCapsule(OverlapCapsuleRadius, OverlapCapsuleHalfHeight);
	FVector OverlapLocation = DownwardCheckHitResult.ImpactPoint + OverlapCapsuleHalfHeight * FVector::UpVector;

	if (GetWorld()->OverlapAnyTestByProfile(
			OverlapLocation, FQuat::Identity,
			FName("Pawn"), OverlapCapsuleShape,
			QueryParams)
	)
	{
		DrawDebugCapsule(GetWorld(), OverlapLocation, OverlapCapsuleHalfHeight, OverlapCapsuleRadius, FQuat::Identity, FColor::Red, false, DrawTime);
		return false;
	}

	LedgeDescription.Location = DownwardCheckHitResult.ImpactPoint;
	LedgeDescription.Rotation = (ForwardCheckHitResult.ImpactNormal * FVector(-1.f, -1.f, 0.f)).ToOrientationRotator();

	return true;
}
