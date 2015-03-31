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

		Air,
		BlackholeThrow,
		Blast,
		Boost,
		Bounce,
		Death,
		HardStop,
		LaserAttack,
		NavCompass,
		RampThrow,
		SharpTurn,
		Walk,

		Everything
	};
}
