// TeleportationEngine.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EyeTrackingComponent.h"
#include "FishEyeComponent.h"
#include "DataFilteringComponent.h"
#include "TeleportationEngine.generated.h"

UCLASS()
class EYECasting_API ATeleportationEngine : public AActor
{
    GENERATED_BODY()

public:
    ATeleportationEngine();

    // Existing declarations...

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    void StartTeleportSequence();

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    void ActivateFisheyeView();

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    FVector GetRelativeGazeLocation();

    // ... add other methods here

protected:
    virtual void BeginPlay() override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UFisheyeComponent* FisheyeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDataFilteringComponent* DataFilteringComponent;

    // ... other component properties

    UPROPERTY(BlueprintAssignable, Category = "Teleportation")
    FTeleportEvent OnTeleportInitiated;
};