// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlatform.h"
#include "PlatformTrigger.h"

// Sets default values
ABasePlatform::ABasePlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DefaultPlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PlatformRoot"));
	SetRootComponent(DefaultPlatformRoot);

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMesh->SetupAttachment(DefaultPlatformRoot);
}

// Called every frame
void ABasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlatformTimeline.TickTimeline(DeltaTime);
}

// Called when the game starts or when spawned
void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = PlatformMesh->GetRelativeLocation();
	PlatformTimeline.SetTimelineLengthMode(TL_LastKeyFrame);

	if (IsValid(TimelineCurve))
	{
		FOnTimelineFloatStatic PlatformMovementTimelineUpdate;
		PlatformMovementTimelineUpdate.BindUObject(this, &ABasePlatform::PlatformTimelineUpdate);
		PlatformTimeline.AddInterpFloat(TimelineCurve, PlatformMovementTimelineUpdate);

		FOnTimelineEventStatic PlatformTimelineFinished;
		PlatformTimelineFinished.BindUObject(this, &ABasePlatform::OnPlatformTimelineFinished);
		PlatformTimeline.SetTimelineFinishedFunc(PlatformTimelineFinished);

		if (PlatformBehavior == EPlatformBehavior::Loop)
		{
			OnPlatformTriggered();
		}
	}

	if (IsValid(PlatformTrigger))
	{
		PlatformTrigger->OnInvocatorActivated.AddDynamic(this, &ABasePlatform::OnPlatformTriggered);
	}
}

void ABasePlatform::PlatformTimelineUpdate(float Alpha)
{
	const FVector PlatformTargetLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	PlatformMesh->SetRelativeLocation(PlatformTargetLocation);
}

void ABasePlatform::OnPlatformTimelineFinished()
{
	switch (PlatformBehavior)
	{
	case EPlatformBehavior::OnDemand:
		if (!PlatformTimeline.IsReversing() && ReturnDelay > 0.f)
		{
			GetWorldTimerManager().SetTimer(ReturnTimerHandle, this, &ABasePlatform::OnPlatformTriggered, ReturnDelay, false);
		}

		break;
	case EPlatformBehavior::Loop:
		OnPlatformTriggered();
		break;
	}
}

void ABasePlatform::OnPlatformTriggered()
{
	GetWorldTimerManager().ClearTimer(ReturnTimerHandle);
	const bool bIsAtStart = FMath::IsNearlyZero(PlatformTimeline.GetPlaybackPosition());

	if (PlatformTimeline.IsReversing() || bIsAtStart)
	{
		PlatformTimeline.Play();
	}
	else
	{
		PlatformTimeline.Reverse();
	}
}
