#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EyeTrackingComponent.h"
#include "FishEyeViewComponent.h"

#include "TeleportationEngine.generated.h"

UCLASS()
class ATeleportationEngine : public AActor
{
    GENERATED_BODY()

public:
    ATeleportationEngine();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UEyeTrackingComponent* EyeTrackingComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBirdEyeViewComponent* BirdEyeViewComponent;

    UFUNCTION(BlueprintCallable, Category = "Teleportation")
    void TeleportPlayer();
};
