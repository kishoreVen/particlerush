// Particle Rush 2014 - 2015

#pragma once

/* Generated Headers +*/
#include "RushFlags.generated.h"

/**
* The main data structure for the Hero with all possible data.
* Find legends for Generic Datatypes at the bottom.
*/
USTRUCT(BlueprintType)
struct FRushFlags
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	float MomentumPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	int32 ChainBoostStage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	int32 ChainBounceStage;
};