// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectXBaseCharacter.generated.h"

class UPXBaseCharacterMovementComponent;

UCLASS(Abstract, NotBlueprintable)
class PROJECTX_API AProjectXBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectXBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void MoveForward(float Value)
	{
	};

	virtual void MoveRight(float Value)
	{
	};

	virtual void Turn(float Value)
	{
	};

	virtual void LookUp(float Value)
	{
	};

	// virtual void Jump() override
	// {
	// };

	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();

	virtual void SwimForward(float Value)
	{
	};

	virtual void SwimRight(float Value)
	{
	};

	virtual void SwimUp(float Value)
	{
	};


	virtual void Tick(float DeltaSeconds) override;

	virtual void Mantle();

	FORCEINLINE UPXBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() { return PXBaseCharacterMovementComponent; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintStart();
	virtual void OnSprintStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Character | Movement")
	void OnSprintEnd();
	virtual void OnSprintEnd_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Movement")
	float SprintSpeed = 800.f;

	virtual bool CanSprint();

	UPXBaseCharacterMovementComponent* PXBaseCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement")
	class ULedgeDetectorComponent* LedgeDetectorComponent;

private:
	void TryChangeSprintState();

	bool bIsSprintRequested = false;
};
