// Particle Rush 2014 - 2015

#pragma once

/* Engine Headers */
#include "ParticleRush.h"

/* Custom Headers */
#include "Generic/DataStructs.h"

/* Generated Headers */
#include "GrindData.generated.h"

/**
* The main data structure for the Grind. 
*/
USTRUCT(BlueprintType)
struct FGrindData
{
	GENERATED_USTRUCT_BODY()
//Grind properties go here. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Basic Data"))
	float Speed; 
	FVector Location;
};

