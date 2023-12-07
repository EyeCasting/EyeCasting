#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ProceduralMeshComponent.h"
#include "FisheyeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FISHEYEVRPLUGIN_API UFisheyeComponent : public UActorComponent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Fisheye")
    float LeftEyeFOV;

    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FVector LeftEyePosePosition;

    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FQuat LeftEyePoseOrientation;

    // Corresponding properties for the right eye
    UPROPERTY(EditAnywhere, Category = "Fisheye")
    float RightEyeFOV;

    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FVector RightEyePosePosition;

    UPROPERTY(EditAnywhere, Category = "Fisheye")
    FQuat RightEyePoseOrientation;

    // Function to create dynamic material instances
    UFUNCTION()
    void CreateDynamicMaterialInstances();

public:
    UFisheyeComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UTextureRenderTarget2D* LeftEyeRenderTarget;

    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UTextureRenderTarget2D* RightEyeRenderTarget;

    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UProceduralMeshComponent* LeftEyeMesh;

    UPROPERTY(VisibleAnywhere, Category = "Fisheye")
    UProceduralMeshComponent* RightEyeMesh;

    // Functions to set up the render targets and meshes will go here
    void InitializeRenderTargets();
    void ConfigureMesh(UProceduralMeshComponent* MeshComponent, const TArray<FVector>& Vertices, const TArray<int32>& Triangles);
};