using UnrealBuildTool;

public class EyeCasting : ModuleRules
{
    public EyeCasting(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            }
            );

        PrivateIncludePaths.AddRange(
            new string[] {
                // ... add other private include paths required here ...
            }
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject", // Include for UObject derived classes
                "Engine", // Include for engine functionalities
                "InputCore", // Include if using input
                "HeadMountedDisplay", // Include for VR functionalities
                "RenderCore", // Include for rendering functionalities
                "RHI", // Include for low-level rendering interfaces
                "ProceduralMeshComponent" // Include for procedural mesh functionalities
                // ... add other public dependencies that you statically link with here ...
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // ... add private dependencies that you statically link with here ...
            }
            );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
            );
    }
}
