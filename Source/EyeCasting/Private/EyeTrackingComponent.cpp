// EyeTrackingComponent.cpp

#include "EyeTrackingComponent.h"
#include "OculusXREyeTrackingComponent.h"

UEyeTrackingComponent::UEyeTrackingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

	eyeTracker.BeginPlay();
	eyeTracker.ClearRotationValues();
}

FVector UEyeTrackingComponent::GetGazeDirection() const
{
    // This will eventually interface with the actual eye tracking hardware/software
    // and return the current gaze direction of the user


    return CurrentGazeDirection;
}

void UEyeTrackingComponent::updateGazeData() const
{
	eyeTracker.bUpdatePosition = true;
	eyeTracker.bUpdateRotation = true;
}

void UEyeTrackingComponent::drawGazeLocation() const
{
	//code that draws the gaze location
}

FVector UEyeTrackingComponent::getRelativeGazeLocation() const
{

}

