#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "FishEyeViewComponent.generated.h"
#include <iterator>

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DataFilteringComponent : public USceneCaptureComponent2D
{
    GENERATED_BODY()

public:
    UDataFilteringComponent();
    UDataFilteringComponent(const FObjectInitializer &ObjectInitializer);
    ~UDataFilteringComponent();

    bool setGazeDataArray(FEyeTrackerGazeData[] gazeDataArray);
    bool setFilteringMode(FString filteringMode);

    FVector getFilteredGazeVector();
    FString getFilteringMode();

private:
    FVector filteredGazeVector;
    FEyeTrackerGazeData[] gazeDataArray;
    FString filteringMode;

    FVector averageLastXVectors(const FVector[] vector);
    FVector medianLastXVectors(const FVector[] vector);
    FVector excludeOutlier(const FVector[] vector);
    FVector excludeMostRecent(const FVector[] vector);
    FVector returnMostRecent(const FVector[] vector);
    void clearGazeDataArray();
};