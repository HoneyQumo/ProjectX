#pragma once

#include "CoreMinimal.h"
#include "ProjectX/Actors/Interactive/InteractiveActor.h"
#include "Ladder.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class PROJECTX_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()

public:
	ALadder();

	virtual void OnConstruction(const FTransform& Transform) override;

	float GetLadderHeight() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float LadderHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float LadderWidth = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float StepsInterval = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float BottomStepOffset = 25.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInstancedStaticMeshComponent* StepsMeshComponent;

	class UBoxComponent* GetLadderInteractionBox() const;
};
