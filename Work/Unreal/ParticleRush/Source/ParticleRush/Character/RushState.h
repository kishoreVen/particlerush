// Particle Rush 2014 - 2015

#pragma once

/* Generated Headers */
#include "RushState.generated.h"

/**
 * Action States for the Rush
 */
UENUM(BlueprintType)
namespace ERushState
{
	enum Type
	{
		None,

		/* Locomotion */
		Air,
		Boost,
		Bounce,
		HardStop,
		SharpTurn,
		Walk,
		/* End Locomotion */

		/* Non Locomotion */
		RampThrow,
		LaserAttack,
		Death,
		NavCompass,
		BlackholeThrow,
		Blast,
		/* End Non Locomotion */

		Everything
	};
}
