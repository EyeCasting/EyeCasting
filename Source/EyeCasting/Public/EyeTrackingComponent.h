// EyeTrackingComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EyeTrackingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EYECasting_API UEyeTrackingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEyeTrackingComponent();

    UFUNCTION(BlueprintCallable, Category = "EyeTracking")
    FVector GetGazeDirection() const;

protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "EyeTracking")
    FVector CurrentGazeDirection;
};