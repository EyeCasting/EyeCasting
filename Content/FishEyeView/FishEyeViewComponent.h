#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "FishEyeViewComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UFishEyeViewComponent : public USceneCaptureComponent2D
{
    GENERATED_BODY()

public:
    UFishEyeViewComponent();

    // Custom logic for Fish's eye view capture
};