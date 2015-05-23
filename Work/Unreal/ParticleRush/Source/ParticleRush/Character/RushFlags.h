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

	/** 
	 * The Difference in Momentum between current frame and the previous frame. 
	 * This can be used to detect if the character is Accelerating or Decelerating
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	float MomentumDiffPercentage;

	/**
	* The Momentum Percentage of the character, calculated as CurrentSpeed over TopSpeed.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	float MomentumPercentage;

	/** 
	* The number of chained boosts, depending on the rhythm of the user.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	int32 ChainBoostStage;

	/**
	* TODO: The number of successive bounces within a given timeframe.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Flags"))
	int32 ChainBounceStage;
};