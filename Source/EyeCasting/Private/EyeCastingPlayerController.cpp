// EyeCastingPlayerController.cpp

#include "EyeCastingPlayerController.h"
#include "TeleportationEngine.h"

void AEyeCastingPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind input events to your handler methods
    InputComponent->BindAction("Teleport", IE_Pressed, this, &AEyeCastingPlayerController::HandleStartTeleport);
    InputComponent->BindAction("Teleport", IE_Released, this, &AEyeCastingPlayerController::HandleEndTeleport);
}

void AEyeCastingPlayerController::HandleStartTeleport()
{
    if (TeleportationEngineInstance)
    {
        TeleportationEngineInstance->StartTeleportSequence();
    }
}

void AEyeCastingPlayerController::HandleEndTeleport()
{
    if (TeleportationEngineInstance)
    {
        // ... call additional functions on TeleportationEngineInstance as needed
    }
}