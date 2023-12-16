// TeleportationEngine.cpp
#include "TeleportationEngine.h"

ATeleportationEngine::ATeleportationEngine()
{
    // Initialize components here
    FisheyeComponent = CreateDefaultSubobject<UFisheyeComponent>(TEXT("FisheyeComponent"));
    DataFilteringComponent = CreateDefaultSubobject<UDataFilteringComponent>(TEXT("DataFilteringComponent"));
    EyeTrackingComponent = CreateDefaultSubobject<UDataFilteringComponent>(TEXT("EyeTrackingComponent"));
    // ... other component initializations
}

void ATeleportationEngine::BeginPlay()
{
    Super::BeginPlay();

    // Initialization code here
    SyncHapticFeedback();
}

bool ATeleportationEngine::StartTeleportSequence()
{
    // check for all necessary components before starting teleport sequence
    if (!FisheyeComponent) 
    {
        /* errors handling: fisheye component not activated */
        return false;
    }
    else if (!EyeTrackingComponent)
    {
        /* errors handling: eye tracking component not activated */
        return false;
    }
    else if (!DataFilteringComponent)
    {
        /* errors handling: data filtering component not activated */
        return false;
    }

    ActivateFisheyeView();

    return true;
}

bool ATeleportationEngine::ActivateFisheyeView()
{
    if (FisheyeComponent)
    {
        /* Alert Player of Change in View with Haptic Feedback */
        APlayerController::PlayHapticEffect(fisheyeHapticEffect, Left, 1.0, 0);
        APlayerController::PlayHapticEffect(fisheyeHapticEffect, Right, 1.0, 0);

        // set parameters accordingly for Screensize, and fisheyeAngleRadians
        FVector2D ScreenSize;

        if (GEngine && GEngine->GameViewport)
        {
            ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
        }

        // set angle for user pov 
        FRotator currentRotation = APlayerCameraManager::GetCameraRotation();
        float FisheyeAngle = currentRotation.NormalizeAxis();

        FisheyeComponent.ApplyFisheyeDistortion(ScreenSize, FishEyeAngle);

        return true;
    }
    return false;
}


// haptics integration
bool ATeleportationEngine::SyncHapticFeedback()
{
    ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> teleportHapticFile(TEXT("/Game/teleportFeedback.haptic");
    ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> fisheyeHapticFile(TEXT("/Game/fisheyeFeedback.haptic");
    ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> selectionHapticFile(TEXT("/Game/selectionFeedback.haptic");
    
    if (teleportHapticFile) {
        teleportationHapticEffect = teleportHapticFile.Object;
    }
    else {
        /* error handling: no haptic file found */
        return false;
    }

    if (fisheyeHapticFile) {
        fisheyeHapticEffect = fisheyeHapticFile.Object;
    }
    else {
        /* error handling: no haptic file found */
        return false;
    }

    if (selectionHapticFile) {
        selectionHapticEffect = selectionHapticFile.Object;
    }
    else {
        /* error handling: no haptic file found */
        return false;
    }

    return true;
}

FVector ATeleportationEngine::GetRelativeGazeLocation()
{

    if (EyeTrackingComponent)
    {
        FVector FilteredGazeDirection = EyeTrackingComponent.GetGazeDirection();

        FVector = Transform::InverseTransformLocation(FilteredGazeDirection);
    }
    return FVector::ZeroVector; // Placeholder
}

bool ATeleportationEngine::TeleportPlayer(FVector TeleportDestination)
{
    bool teleportSuccessful = false;

    if (ConfirmTeleportDestination(TeleportDestination))
    {
        /* Alert Player of Change in View with Haptic Feedback */
        APlayerController::PlayHapticEffect(teleportHapticEffect, Left, 1.0, 0);
        APlayerController::PlayHapticEffect(teleportHapticEffect, Right, 1.0, 0);

        if (InitiateFOVReduction())
        {
            teleportSuccessful = TraverseToGazeLocation(TeleportDestination);
        }
        else {
            // error handling - FOV not reduced
        }
    }
    else {
        // error handling - invalid destination
        return false;
    }

    RestoreNormalFOV();

    if (teleportSuccessful)
        return true;
    else
        return false;
}


bool ATeleportationEngine::ConfirmTeleportDestination(FVector TeleportDestination)
{
    // verify that the player is able to move to the destination

    return false;
}

bool ATeleportationEngine::InitiateFOVReduction()
{
    return false;
}

bool ATeleportationEngine::RestoreNormalFOV()
{
    return false;
}

bool ATeleportationEngine::TraverseToGazeLocation(FVector TeleportDestination)
{

    return false;
}

