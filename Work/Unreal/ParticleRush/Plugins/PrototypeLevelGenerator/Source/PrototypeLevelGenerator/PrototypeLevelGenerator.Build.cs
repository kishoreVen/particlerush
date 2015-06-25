// Some copyright should be here...

using UnrealBuildTool;

public class PrototypeLevelGenerator : ModuleRules
{
	public PrototypeLevelGenerator(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"PrototypeLevelGenerator/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"PrototypeLevelGenerator/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Engine",
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"Slate", 
                "SlateCore",
                "Core",
				"EditorStyle",
				"CoreUObject",

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
