// Some copyright should be here...

using UnrealBuildTool;

public class LevelDesigner : ModuleRules
{
	public LevelDesigner(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"LevelDesigner/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"LevelDesigner/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
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
                "EditorWidgets",
                "PropertyEditor",
                "ParticleRush",
                "Landscape",
                "LandscapeEditor",
                "ClassViewer",
                "RenderCore",
				"AssetTools"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				"AssetTools"
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
