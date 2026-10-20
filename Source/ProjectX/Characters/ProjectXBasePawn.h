// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectXBasePawn.generated.h"

UCLASS()
class PROJECTX_API AProjectXBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AProjectXBasePawn();

	UPROPERTY(VisibleAnywhere)
	class UPawnMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetInputForward() { return InputForward; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetInputRight() { return InputRight; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* FirstPersonCamera;

private:
	UFUNCTION()
	void OnBlendComplete();

	void SwitchCamera();

	AActor* CurrentViewActor;

	float InputForward = 0.f;
	float InputRight = 0.f;
};
