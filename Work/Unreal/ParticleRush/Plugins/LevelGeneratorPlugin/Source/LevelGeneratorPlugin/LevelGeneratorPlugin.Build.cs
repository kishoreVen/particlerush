// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class LevelGeneratorPlugin : ModuleRules
{
	public LevelGeneratorPlugin(TargetInfo Target)
	{
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				"LevelGeneratorPlugin/Private",
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
				"UnrealEd",
				"InputCore",
				"Core",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"CoreUObject",
                "LevelEditor",
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