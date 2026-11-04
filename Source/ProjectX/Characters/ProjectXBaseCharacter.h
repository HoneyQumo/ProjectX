// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectXBaseCharacter.generated.h"

class AInteractiveActor;

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MantlingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveVector* MantlingCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.f, UIMin = 0.f))
	float AnimationCorrectionXY = 65.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.f, UIMin = 0.f))
	float AnimationCorrectionZ = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.f, UIMin = 0.f))
	float MinHeight = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.f, UIMin = 0.f))
	float MaxHeight = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.f, UIMin = 0.f))
	float MinHeightStartTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.f, UIMin = 0.f))
	float MaxHeightStartTime = 0.f;
};

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

	virtual bool CanJumpInternal_Implementation() const override;

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

	FORCEINLINE UPXBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const { return PXBaseCharacterMovementComponent; }

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings HighMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling")
	FMantlingSettings LowMantleSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Movement | Mantling", meta = (ClampMin = 0.f, UIMin = 0.f))
	float LowMantleMaxHeight = 125.f;

private:
	void TryChangeSprintState();

	bool bIsSprintRequested = false;

	const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;

	TArray<AInteractiveActor*> AvailableInteractiveActors;
};
