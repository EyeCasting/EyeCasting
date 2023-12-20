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
    ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> fovHapticFile(Text("/Game/fovFeedback.hatpic");

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

    if (fovHapticFile) {
        fovHapticEffect = fovHapticFile.Object;
    }
    else {
        /* error handling: no haptic file found */
        return false;
    }

    return true;
}

FVector ATeleportationEngine::GetRelativeGazeLocation()
{
    FVector gazeLocation;

    if (EyeTrackingComponent)
    {
        FVector FilteredGazeDirection = EyeTrackingComponent.GetGazeDirection();

        gazeLocation = Transform::InverseTransformLocation(FilteredGazeDirection);
    }
    else {
        gazeLocation = FVector::ZeroVector;
    }

    teleportGazeLocation = gazeLocation;
    return gazeLocation;
}

bool ATeleportationEngine::TeleportPlayer(FVector TeleportDestination)
{
    bool teleportSuccessful = false;

    confirmTeleport = ConfirmTeleportDestination(TeleportDestination);

    if (confirmTeleport)
    {
        /* Alert Player of Change in View with Haptic Feedback */
        APlayerController::PlayHapticEffect(teleportHapticEffect, Left, 1.0, 0);
        APlayerController::PlayHapticEffect(teleportHapticEffect, Right, 1.0, 0);

        if (reducedFOV)
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

    bool playerFOVnormal;
    playerFOVnormal = RestoreNormalFOV();

    if (!playerFOVnormal) {
        // IMPORTANT error handling
        // need to return player FOV back to normal
    }

    if (teleportSuccessful)
        return true;
    else
        return false;
}

void ATeleportationEngine::CompleteTeleportSequence()
{
    if (!reducedFOV) {
        // any final steps in teleportation sequence
    }
    else {
        // debug - restore user fov before exiting
    }
}

bool ATeleportationEngine::ConfirmTeleportDestination(FVector TeleportDestination)
{
    confirmTeleport = false;
    // verify that the player is able to move to the destination
    if (TeleportDestination != FVector::ZeroVector)
    {
        // verify teleport destination is movable to

        confirmTeleport = true;
    }

    return confirmTeleport;
}

bool ATeleportationEngine::InitiateFOVReduction()
{
    if (confirmTeleport) {

        /* Alert Player of Change in View with Haptic Feedback */
        APlayerController::PlayHapticEffect(fovHapticEffect, Left, 1.0, 0);
        APlayerController::PlayHapticEffect(fovHapticEffect, Right, 1.0, 0);
        
        // reduce PixelDensity to add blur to user's pov
        vr.PixelDensity = 0.5;  // value needs testing (1 is clear, 0 blurry)

        // apply other fov reduction techniques

        // set reducedFOV if fov successfully reduced
        reducedFOV = true;
    }
    return reducedFOV;
}

bool ATeleportationEngine::RestoreNormalFOV()
{
    // restore pixel density to default value
    vr.PixelDensity = 1.0; 

    // restore other fov reduction techniques

    // set reducedFOV if fov successfully restored
    reducedFOV = false;
   
    return !reducedFOV;
}

bool ATeleportationEngine::TraverseToGazeLocation(FVector TeleportDestination)
{
    if (confirmTeleport && reducedFOV)
    {
        UWorld currentWorld = UWorld::GetWorld();
        APlayerController currentPlyrCtrl;
        ACharacter currCharacter;
        FVector currLocation;
        
        if (currentWorld)
            currentPlyrCtrl = currentWorld->GetFirstLocalPlayerController();
        else
            return false;

        if (currentPlyrCtrl)
            currCharacter = currentPlyrCtrl->GetPawn();
        else
            return false;

        if (currCharacter)
            currLocation = currCharacter->GetActorLocation();
        else
            return false;

        FVector travelVector = teleportGazeDestination - currLocation;

        // actually moving actor to teleportGazeDestination

        return true;
    }

    return false;
}

