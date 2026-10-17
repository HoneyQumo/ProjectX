// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ProjectXBasePawnMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTX_API UProjectXBasePawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	void JumpStart();

protected:
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1200.f;

	UPROPERTY(EditAnywhere)
	bool bEnableGravity = true;

	UPROPERTY(EditAnywhere)
	float InitialJumpVelocity = 500.f;

private:
	FVector VerticalVelocity = FVector::ZeroVector;
	bool bIsFalling = false;
};
