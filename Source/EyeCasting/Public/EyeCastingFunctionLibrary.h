// EyeCastingFunctionLibrary.h

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "EyeCastingFunctionLibrary.generated.h"

UCLASS()
class EYECasting_API UEyeCastingFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // BlueprintCallable function to calculate the teleport destination
    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    static FVector CalculateTeleportDestination(/* parameters if needed */);
};