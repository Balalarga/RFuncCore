using UnrealBuildTool;
using System.IO;

public class RFuncCoreLib : ModuleRules
{
    public RFuncCoreLib(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;
        bUseRTTI = true;
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );

        PublicIncludePaths.Add(ModuleDirectory);

        // ThirdParty
        var thirdPartyDir = Path.Combine(PluginDirectory, "ThirdParty");
        PublicIncludePaths.AddRange(
	        new string[]
	        {
		        Path.Combine(thirdPartyDir, "glm_1.0.1"),
		        Path.Combine(thirdPartyDir, "OpenCL", "include"),
	        }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
	        PublicAdditionalLibraries.AddRange(
		        new string[]
		        {
			        Path.Combine(thirdPartyDir, "OpenCL", "lib", "Win64", "OpenCL.lib"),
		        }
	        );
        }
    }
}