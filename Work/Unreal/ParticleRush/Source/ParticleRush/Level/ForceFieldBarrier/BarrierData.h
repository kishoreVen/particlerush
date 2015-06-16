// Particle Rush 2014 - 2015

#pragma once

/* Engine Headers */
#include "ParticleRush.h"

/* Custom Headers */
#include "Generic/DataStructs.h"

/* Generated Headers */
#include "BarrierData.generated.h"

/**
* The main data structure for the Accelerator Track`.
*/
USTRUCT(BlueprintType)
struct FBarrierData
{
	GENERATED_USTRUCT_BODY()

	/* Holds spline index */
	FVector BarrierLocation;

	
	FBarrierData()
	{
		BarrierLocation = FVector::ZeroVector;		
	}
};