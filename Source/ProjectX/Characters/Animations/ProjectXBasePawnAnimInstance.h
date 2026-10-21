// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectXBasePawnAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API UProjectXBasePawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base Pawn Animation Instance")
	float InputForward;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base Pawn Animation Instance")
	float InputRight;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Base Pawn Animation Instance")
	bool bIsInAir;

private:
	TWeakObjectPtr<class AProjectXBasePawn> CachedBasePawn;
};
