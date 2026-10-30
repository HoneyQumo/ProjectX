#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeDetectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FLedgeDescription
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ledge description")
	FRotator Rotation;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTX_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool DetectLedge(OUT FLedgeDescription& LedgeDescription);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.f, ClampMin = 0.f))
	float MinimumLedgeHeight = 40.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.f, ClampMin = 0.f))
	float MaximumLedgeHeight = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Detection settings", meta = (UIMin = 0.f, ClampMin = 0.f))
	float ForwardCheckDistance = 100.f;

private:
	TWeakObjectPtr<class ACharacter> CachedCharacterOwner;
};
