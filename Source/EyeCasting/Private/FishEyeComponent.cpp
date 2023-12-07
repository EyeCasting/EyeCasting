#include "FisheyeComponent.h"
#include "EyeCastingPlayerController.h"
#include "EyeTrackingComponent.h"
#include "Engine/Engine.h"

UFisheyeComponent::UFisheyeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    LeftEyeFOV = 95.0f;
    LeftEyePosePosition = FVector(-0.032f, 0.0f, 0.0f);
    LeftEyePoseOrientation = FQuat(0.f, 0.f, 0.f, 1.f);

    RightEyeFOV = 95.0f;
    RightEyePosePosition = FVector(0.032f, 0.0f, 0.0f);
    RightEyePoseOrientation = FQuat(0.f, 0.f, 0.f, 1.f);

    LeftEyeRenderTarget = nullptr;
    RightEyeRenderTarget = nullptr;
    LeftEyeMesh = nullptr;
    RightEyeMesh = nullptr;
}

void UFisheyeComponent::BeginPlay()
{
    Super::BeginPlay();

    InitializeRenderTargets();
    CreateDynamicMaterialInstances();
}

void UFisheyeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateMaterialParameters(DeltaTime);
}

void UFisheyeComponent::InitializeRenderTargets()
{
    if (!LeftEyeRenderTarget)
    {
        LeftEyeRenderTarget = NewObject<UTextureRenderTarget2D>(this);
        LeftEyeRenderTarget->InitCustomFormat(1832, 1920, PF_FloatRGBA, false);
        LeftEyeRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
        LeftEyeRenderTarget->bAutoGenerateMips = false;
        LeftEyeRenderTarget->FieldOfView = LeftEyeFOV;
        LeftEyeRenderTarget->UpdateResource();
    }

    if (!RightEyeRenderTarget)
    {
        RightEyeRenderTarget = NewObject<UTextureRenderTarget2D>(this);
        RightEyeRenderTarget->InitCustomFormat(1832, 1920, PF_FloatRGBA, false);
        RightEyeRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
        RightEyeRenderTarget->bAutoGenerateMips = false;
        RightEyeRenderTarget->FieldOfView = RightEyeFOV;
        RightEyeRenderTarget->UpdateResource();
    }
}

void UFisheyeComponent::CreateDynamicMaterialInstances()
{
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/FisheyeMaterial.FisheyeMaterial"));
    UMaterialInterface* BaseMaterial = MaterialFinder.Object;

    if (BaseMaterial)
    {
        if (!LeftEyeMesh)
        {
            LeftEyeMesh = NewObject<UProceduralMeshComponent>(this);
            LeftEyeMesh->RegisterComponent();
        }
        LeftEyeMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, LeftEyeMesh);
        LeftEyeMesh->SetMaterial(0, LeftEyeMaterialInstance);
        LeftEyeMaterialInstance->SetTextureParameterValue(FName("RenderTexture"), LeftEyeRenderTarget);

        if (!RightEyeMesh)
        {
            RightEyeMesh = NewObject<UProceduralMeshComponent>(this);
            RightEyeMesh->RegisterComponent();
        }
        RightEyeMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, RightEyeMesh);
        RightEyeMesh->SetMaterial(0, RightEyeMaterialInstance);
        RightEyeMaterialInstance->SetTextureParameterValue(FName("RenderTexture"), RightEyeRenderTarget);
    }
}

void UFisheyeComponent::UpdateMaterialParameters(float DeltaTime)
{
    FVector2D ViewportSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }

    AEyeCastingPlayerController* EyeCastingPlayerController = Cast<AEyeCastingPlayerController>(GetWorld()->GetFirstPlayerController());
    if (EyeCastingPlayerController)
    {
        UEyeTrackingComponent* EyeTrackingComponent = EyeCastingPlayerController->FindComponentByClass<UEyeTrackingComponent>();
        if (EyeTrackingComponent)
        {
            FVector GazeDirection = EyeTrackingComponent->GetGazeDirection();
            FVector2D CurrentScreenLocation = ConvertGazeDirectionToScreenLocation(GazeDirection);
            FVector2D FisheyeUV = ApplyFisheyeDistortion(ConvertScreenLocationToUV(CurrentScreenLocation), ViewportSize, PI / 2.0f); // fisheye angle of 90 degrees in radians

            // Update the material parameters
            if (LeftEyeMaterialInstance)
            {
                LeftEyeMaterialInstance->SetVectorParameterValue(FName("FisheyeUV"), FLinearColor(FisheyeUV.X, FisheyeUV.Y, 0.0f, 0.0f));
            }
            if (RightEyeMaterialInstance)
            {
                RightEyeMaterialInstance->SetVectorParameterValue(FName("FisheyeUV"), FLinearColor(FisheyeUV.X, FisheyeUV.Y, 0.0f, 0.0f));
            }
        }
    }
}

FVector2D UFisheyeComponent::ApplyFisheyeDistortion(const FVector2D& UV, const FVector2D& ScreenSize, float FisheyeAngleRadians)
{
    FVector2D AspectRatio = FVector2D(ScreenSize.X / ScreenSize.Y, 1.0f);
    FVector2D CenterUVs = FVector2D(0.5f, 0.5f);
    FVector2D RelativePosition = (UV - CenterUVs) * AspectRatio;
    float RelativeDistance = RelativePosition.Size();
    FVector2D ViewportScale = FVector2D(2.0f / ScreenSize.X, 2.0f / ScreenSize.Y) * AspectRatio;
    FVector2D ScaledRelativePosition = RelativePosition * ViewportScale;
    float BarrelDistortionAngle = FMath::Atan(RelativeDistance);
    FVector2D BarrelDistortion = FVector2D(ScaledRelativePosition.X, ScaledRelativePosition.Y / FMath::Cos(BarrelDistortionAngle));
    float PinchDistortionAngle = FisheyeAngleRadians * RelativeDistance;
    FVector2D PinchDistortion = FVector2D(ScaledRelativePosition.X * FMath::Tan(PinchDistortionAngle), ScaledRelativePosition.Y);
    FVector2D DistortedUV = BarrelDistortion + PinchDistortion;
    DistortedUV = DistortedUV.ClampAxes(0.0f, 1.0f);
    return DistortedUV;
}

FVector2D UFisheyeComponent::ConvertScreenLocationToUV(const FVector2D& ScreenLocation)
{
    FVector2D ViewportSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }
    return ScreenLocation / ViewportSize;
}

FVector2D UFisheyeComponent::ConvertUVToScreenLocation(const FVector2D& UV)
{
    FVector2D ViewportSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }
    return UV * ViewportSize;
}

FVector2D UFisheyeComponent::ConvertGazeDirectionToScreenLocation(const FVector& GazeDirection)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        FVector2D ScreenLocation;
        FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
        FVector ProjectedLocation = PlayerLocation + GazeDirection * 1000.0f; // Project the gaze direction

        PlayerController->ProjectWorldLocationToScreen(ProjectedLocation, ScreenLocation);
        return ScreenLocation;
    }

    return FVector2D(0.5f, 0.5f); // Default to screen center if unable to project
}