// EyeTrackingComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EyeTrackingComponent.generated.h"
#include "OculusXREyeTrackingComponent.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EYECasting_API UEyeTrackingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
	UOculusXREyeTrackingComponent eyeTracker;

    UEyeTrackingComponent();

    UFUNCTION(BlueprintCallable, Category = "EyeTracking")
    FVector GetGazeDirection() const;
	void drawGazeLocation() const;
	void updateGazeData() const;

protected:
	FVector getRelativeGazeLocation() const;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "EyeTracking")
    FVector CurrentGazeDirection;
};