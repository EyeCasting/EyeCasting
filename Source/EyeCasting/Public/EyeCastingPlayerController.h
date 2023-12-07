// EyeCastingPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EyeCastingPlayerController.generated.h"

UCLASS()
class EYECasting_API AEyeCastingPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;

private:
    // Methods to handle input
    void HandleStartTeleport();
    void HandleEndTeleport();
};