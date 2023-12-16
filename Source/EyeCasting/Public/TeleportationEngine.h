// TeleportationEngine.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EyeTrackingComponent.h"
#include "FishEyeComponent.h"
#include "DataFilteringComponent.h"

// Unreal Libraries
#include "UObject/ConstructorHelper.h"
#include "Math/Vector.h"    // FVector
#include "CoreFwd.h"        //FRotator
#include "Math/Transform.h"

// Unreal Teleport Library
#include "TeleportationEngine.generated.h"

// Unreal Camera Library
#include "Camera/PlayerCameraManager.h"

// Unreal Haptics Library
#include "GameFramework/PlayerController.h"
#include "Haptics/HapticFeedbackEffect_Base.h"

UCLASS()
class EYECasting_API ATeleportationEngine : public AActor
{
    GENERATED_BODY()

public:
    ATeleportationEngine();

    // Existing declarations...

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    bool StartTeleportSequence();

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    bool ActivateFisheyeView();

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    FVector GetRelativeGazeLocation();

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    bool TeleportPlayer();

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    void CompleteTeleportSequence();

    // figure out input from controller to trigger teleportation

protected:
    virtual void BeginPlay() override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UFisheyeComponent* FisheyeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UEyeTrackingComponent* EyeTrackingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDataFilteringComponent* DataFilteringComponent;

    // ... other component properties

    UPROPERTY(BlueprintAssignable, Category = "Teleportation")
    FTeleportEvent OnTeleportInitiated;

private:
    bool SyncHapticFeedback();
    bool ConfirmTeleportDestination(FVector TeleportDestination);
    bool InitiateFOVReduction();
    bool RestoreNormalFOV();
    bool TraverseToGazeLocation(FVector TeleportDestination);

    UHapticFeedbackEffect_Base teleportHapticEffect;
    UHapticFeedbackEffect_Base fisheyeHapticEffect;
    UHapticFeedbackEffect_Base selectionHapticEffect;
};