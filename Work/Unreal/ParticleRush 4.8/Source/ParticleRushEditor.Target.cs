// Particle Rush 2014 - 2015

using UnrealBuildTool;
using System.Collections.Generic;

public class ParticleRushEditorTarget : TargetRules
{
	public ParticleRushEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "ParticleRush" } );
	}
}
