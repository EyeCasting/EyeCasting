#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EyeTrackingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEyeTrackingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEyeTrackingComponent();

    // Methods to interact with the eye-tracking hardware or software
};
