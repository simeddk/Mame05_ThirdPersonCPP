using UnrealBuildTool;

public class ThirdPersonCPP : ModuleRules
{
	public ThirdPersonCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"AIModule",
			"GameplayTasks"
		});


		PublicIncludePaths.Add(ModuleDirectory);

	}
}
