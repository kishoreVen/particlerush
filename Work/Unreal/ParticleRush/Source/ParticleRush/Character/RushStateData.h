// Particle Rush 2014 - 2015

#pragma once

/* Custom Headers */
#include "RushStateLayer.h"

/* Generated Headers */
#include "RushStateData.generated.h"

/** 
* Data for each Rush State
*/
USTRUCT(BlueprintType)
struct FRushStateData
{
	GENERATED_USTRUCT_BODY()

	FRushStateData()
	{
		LayerIndex = ERushStateLayer::None;
		StateName = "None";
	}

	uint32_t	LayerIndex;
	FString		StateName;
};