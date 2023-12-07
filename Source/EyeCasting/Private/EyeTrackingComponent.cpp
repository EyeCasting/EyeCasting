// EyeTrackingComponent.cpp

#include "EyeTrackingComponent.h"

UEyeTrackingComponent::UEyeTrackingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

FVector UEyeTrackingComponent::GetGazeDirection() const
{
    // This will eventually interface with the actual eye tracking hardware/software
    // and return the current gaze direction of the user
    return CurrentGazeDirection;
}

