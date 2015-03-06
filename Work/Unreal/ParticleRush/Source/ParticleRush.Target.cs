// Particle Rush 2014 - 2015

using UnrealBuildTool;
using System.Collections.Generic;

public class ParticleRushTarget : TargetRules
{
	public ParticleRushTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
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
