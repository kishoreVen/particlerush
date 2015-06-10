// Particle Rush 2014 - 2015

#pragma once

/* Engine Headers */
#include "ParticleRush.h"

/* Custom Headers */
#include "Generic/DataStructs.h"

/* Generated Headers */
#include "TrackData.generated.h"

/**
* The main data structure for the Accelerator Track`.
*/
USTRUCT(BlueprintType)
struct FTrackData
{
	GENERATED_USTRUCT_BODY()

	/* The mesh that will be used as track for this spline */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	UStaticMesh* MainTrackMesh;
};