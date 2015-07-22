// Particle Rush 2014 - 2015

#pragma once

/* Generated Headers +*/
#include "InputDOF.generated.h"

/**
* An enum of a list of rush actions that are input controlled.
* Each enum item is associated (or assigned) with the corresponding power of 2, to avoid repeated calculation.
*/
UENUM(BlueprintType)
namespace EInputDOF
{
	enum Type
	{
		MOVE 		= 1,
		TURN 		= 2,
		BRAKE		= 4,
		SHARP_TURN 	= 8,
		BOOST 		= 16,
		JUMP 		= 32,
		EVERYTHING  = 64
	};	
}