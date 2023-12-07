#include "FisheyeComponent.h"

UFisheyeComponent::UFisheyeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Update FOV and eye pose with Meta Quest Pro specs
    LeftEyeFOV = 95.0f; // Adjusted Horizontal FOV for Meta Quest Pro
    LeftEyePosePosition = FVector(-0.032f, 0.0f, 0.0f); // Updated based on Meta Quest Pro IPD
    LeftEyePoseOrientation = FQuat(0.f, 0.f, 0.f, 1.f);

    // Initialize corresponding properties for the right eye
    RightEyeFOV = 95.0f; // Same FOV for Right Eye
    RightEyePosePosition = FVector(0.032f, 0.0f, 0.0f); // Mirrored position for Right Eye
    RightEyePoseOrientation = FQuat(0.f, 0.f, 0.f, 1.f); // Same orientation for Right Eye

    // Initialize render target pointers
    LeftEyeRenderTarget = nullptr;
    RightEyeRenderTarget = nullptr;

    // Initialize mesh component pointers
    LeftEyeMesh = nullptr;
    RightEyeMesh = nullptr;
}

void UFisheyeComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeRenderTargets();

    ConfigureMesh(LeftEyeMesh, LeftEyeVertices, LeftEyeTriangles);
    ConfigureMesh(RightEyeMesh, RightEyeVertices, RightEyeTriangles);
    // Create dynamic material instances after meshes are configured
    CreateDynamicMaterialInstances();
}

void UFisheyeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Dynamic updates per frame
}

void UFisheyeComponent::InitializeRenderTargets()
{
    // Configure left eye render target with Meta Quest Pro specifications
    if (!LeftEyeRenderTarget)
    {
        LeftEyeRenderTarget = NewObject<UTextureRenderTarget2D>(this);
        if (LeftEyeRenderTarget)
        {
            LeftEyeRenderTarget->InitCustomFormat(1832, 1920, PF_FloatRGBA, false); // Updated resolution
            LeftEyeRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
            LeftEyeRenderTarget->bAutoGenerateMips = false;
            LeftEyeRenderTarget->FieldOfView = LeftEyeFOV;
            // Additional configuration as needed
        }
    }
    // Configure right eye render target with similar settings
    if (!RightEyeRenderTarget)
    {
        RightEyeRenderTarget = NewObject<UTextureRenderTarget2D>(this);
        if (RightEyeRenderTarget)
        {
            RightEyeRenderTarget->InitCustomFormat(1832, 1920, PF_FloatRGBA, false); // Same resolution as Left Eye
            RightEyeRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
            RightEyeRenderTarget->bAutoGenerateMips = false;
            RightEyeRenderTarget->FieldOfView = RightEyeFOV;
            // Additional configuration as needed
        }
    }
    // Configure right eye render target
    RightEyeRenderTarget = NewObject<UTextureRenderTarget2D>(this);
    RightEyeRenderTarget->InitCustomFormat(1376, 1472, PF_FloatRGBA, false);
    
    // Register the render targets so they are properly updated
    if (LeftEyeRenderTarget && !LeftEyeRenderTarget->IsPendingKill())
    {
        LeftEyeRenderTarget->UpdateResource();
    }
    if (RightEyeRenderTarget && !RightEyeRenderTarget->IsPendingKill())
    {
        RightEyeRenderTarget->UpdateResource();
    }
    
    // Set additional parameters like FOV, orientation, etc.
}

void UFisheyeComponent::ConfigureMesh(UProceduralMeshComponent* MeshComponent, const TArray<FVector>& Vertices, const TArray<int32>& Triangles)
{
    // Check if the procedural mesh component exists and create if not
    if (!MeshComponent)
    {
        MeshComponent = NewObject<UProceduralMeshComponent>(this);
        if (MeshComponent)
        {
            MeshComponent->RegisterComponent(); // Registers component with the world, so it shows up in the game
        }
    }

    // Create the mesh section
    if (MeshComponent && !MeshComponent->IsPendingKill())
    {
        MeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

        // Set mesh properties for the left eye
        MeshComponent->SetWorldLocation(LeftEyePosePosition);
        MeshComponent->SetWorldRotation(LeftEyePoseOrientation.Rotator());

        // Set mesh properties for the right eye
        if (MeshComponent == RightEyeMesh) // Check if it's the right eye mesh
        {
            MeshComponent->SetWorldLocation(RightEyePosePosition);
            MeshComponent->SetWorldRotation(RightEyePoseOrientation.Rotator());
        }
    }
}

void UFisheyeComponent::CreateDynamicMaterialInstances()
{
    // Find the material in the content folder using ConstructorHelpers if needed
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/FisheyeMaterial.FisheyeMaterial"));
    UMaterialInterface* BaseMaterial = MaterialFinder.Object;

    // Create dynamic material instance for left eye
    if (BaseMaterial && LeftEyeMesh)
    {
        UMaterialInstanceDynamic* LeftEyeMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, LeftEyeMesh);
        LeftEyeMesh->SetMaterial(0, LeftEyeMaterialInstance);
        LeftEyeMaterialInstance->SetTextureParameterValue(FName("RenderTexture"), LeftEyeRenderTarget);
        // Set other material parameters as needed for distortion
        // ...
    }

    // Create dynamic material instance for right eye
    if (BaseMaterial && RightEyeMesh)
    {
        UMaterialInstanceDynamic* RightEyeMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, RightEyeMesh);
        RightEyeMesh->SetMaterial(0, RightEyeMaterialInstance);
        RightEyeMaterialInstance->SetTextureParameterValue(FName("RenderTexture"), RightEyeRenderTarget);
        // Set other material parameters as needed for distortion
        // ...
    }
}