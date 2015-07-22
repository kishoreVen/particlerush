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

	/* Holds spline index */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Spline Sync"))
	FVector SplinePointLocation;

	/* Banking angle for curvature of tracks at turns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Mesh", UIMin = -90.0f, UIMax = 90.0f))
	float BankingAngle;

	/* Banking angle for curvature of tracks at turns */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Mesh"))
	bool IsTunnel;

	/* The scale according to the mesh (not unreal units), which will specify the track width (as X) and thickness (as Y) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Mesh"))
	FVector2D TrackScale;

	FTrackData()
	{
		SplinePointLocation		= FVector::ZeroVector;
		BankingAngle			= 0.0f;
		TrackScale				= FVector2D(1.0f, 1.0f);
	}
};