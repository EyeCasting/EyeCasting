// TeleportationEngine.cpp
#include "TeleportationEngine.h"

ATeleportationEngine::ATeleportationEngine()
{
    // Initialize components here
    FisheyeComponent = CreateDefaultSubobject<UFisheyeComponent>(TEXT("FisheyeComponent"));
    DataFilteringComponent = CreateDefaultSubobject<UDataFilteringComponent>(TEXT("DataFilteringComponent"));
    // ... other component initializations
}

void ATeleportationEngine::BeginPlay()
{
    Super::BeginPlay();
    // Initialization code here
}

bool ATeleportationEngine::StartTeleportSequence()
{
    // Implement the logic to start the teleportation sequence

    if (!ActivateFisheyeView()) 
    {
        // errors in activating fisheye view - troubleshooting
        return false;
    }

    if (!SyncHapticFeedback())
    {
        // errors in activating haptic sync
        return false;
    }

    return true;
}

bool ATeleportationEngine::ActivateFisheyeView()
{
    if (FisheyeComponent)
    {
        // Call a method on the FisheyeComponent to activate the view

        // set parameters accordingly for UV, screensize, and fisheyeAngleRadians
        
        FVector2D UV, ScreenSize;

        if (GEngine && GEngine->GameViewport)
        {
            ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
        }

        // set parameter FVector2D UV

        // set angle for user pov 
        float FisheyeAngle = 3.14;  // 180 degrees initially

        FisheyeComponent.ApplyFisheyeDistortion(UV, ScreenSize, FishEyeAngle);

        return true;
    }
    return false;
}


bool ATeleportationEngine::SyncHapticFeedback()
{
    // haptics integration
    return true;
}

FVector ATeleportationEngine::GetRelativeGazeLocation()
{

    if (EyeTrackingComponent)
    {
        FVector FilteredGazeDirection = EyeTrackingComponent.GetGazeDirection();

        // convert gaze direction into a teleportation destination
    }
    return FVector::ZeroVector; // Placeholder
}

bool ATeleportationEngine::TeleportPlayer(FVector TeleportDestination)
{
    bool teleportSuccessful = false;

    if (ConfirmTeleportDestination(TeleportDestination))
    {
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
    // for now just using Unreal's teleport function
    return false;
}

