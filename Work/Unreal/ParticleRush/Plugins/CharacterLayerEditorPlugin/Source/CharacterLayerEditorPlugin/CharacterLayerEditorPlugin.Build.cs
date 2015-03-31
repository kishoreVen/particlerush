// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class CharacterLayerEditorPlugin : ModuleRules
	{
		public CharacterLayerEditorPlugin(TargetInfo Target)
		{
			PrivateIncludePaths.AddRange(new string[] { "CharacterLayerEditorPlugin/Private" });
			PublicIncludePaths.AddRange(new string[] { "CharacterLayerEditorPlugin/Public" });
 
			PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core" });
		}
	}
}