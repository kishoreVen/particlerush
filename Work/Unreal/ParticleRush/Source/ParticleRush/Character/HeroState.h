// Particle Rush 2014 - 2015

#pragma once

/* Generated Headers */
#include "HeroState.generated.h"

/**
 * Action States for the Rush
 */
UENUM(BlueprintType)
namespace EHeroState
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
