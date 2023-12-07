#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FisheyeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EYECasting_API UFisheyeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFisheyeComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FVector2D ApplyFisheyeDistortion(const FVector2D& UV, const FVector2D& ScreenSize, float FisheyeAngleRadians);
    FVector2D ConvertGazeDirectionToScreenLocation(const FVector& GazeDirection);

protected:
    UPROPERTY(EditAnywhere, Category = "Fisheye")
    float LeftEyeFOV;
    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FVector LeftEyePosePosition;
    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FQuat LeftEyePoseOrientation;

    UPROPERTY(EditAnywhere, Category = "Fisheye")
    float RightEyeFOV;
    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FVector RightEyePosePosition;
    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FQuat RightEyePoseOrientation;

    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UTextureRenderTarget2D* LeftEyeRenderTarget;
    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UTextureRenderTarget2D* RightEyeRenderTarget;
    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UProceduralMeshComponent* LeftEyeMesh;
    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UProceduralMeshComponent* RightEyeMesh;

private:
    UMaterialInstanceDynamic* LeftEyeMaterialInstance;
    UMaterialInstanceDynamic* RightEyeMaterialInstance;

    void InitializeRenderTargets();
    void CreateDynamicMaterialInstances();
    void UpdateMaterialParameters(float DeltaTime);
    FVector2D ConvertScreenLocationToUV(const FVector2D& ScreenLocation);
    FVector2D ConvertUVToScreenLocation(const FVector2D& UV);
};
