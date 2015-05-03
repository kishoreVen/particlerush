// Particle Rush 2014 - 2015

#pragma once

/* Custom Headers */
#include "RushState.h"
#include "RushStateData.h"
#include "RushStateMatrix.h"

/* Generated Headers */
#include "RushStateLayerData.generated.h"

/** 
* Data for each Rush State
*/
USTRUCT(BlueprintType)
struct FRushStateLayerData
{
	GENERATED_USTRUCT_BODY()

	FRushStateLayerData()
	{
		Layer = ERushStateLayer::None;
		LayerName = "None";
		ActiveState = FRushStateData();
		BlendingOutState = ERushState::None;
	}

	FRushStateLayerData(uint32_t layer, FString layerName, FRushStateData activeState)
	{
		Layer = layer;
		LayerName = layerName;
		ActiveState = activeState;
		BlendingOutState = ERushState::None;
	}

	void Update(float DeltaSeconds)
	{

	}

	bool RequestStateChange(uint32_t toState)
	{
		if (toState > LayerEndIndices[Layer] || toState < LayerStartIndices[Layer])
			return false;

		if (ActiveState.StateIndex == ERushState::None)
			return true;

		uint32_t layerState = toState - LayerStartIndices[Layer];
		uint32_t currentState = ActiveState.StateIndex - LayerStartIndices[Layer];

		return LayerMatrix[Layer][layerState][currentState];
	}

	void SetState(FRushStateData newState)
	{
		ActiveState = newState;
	}

	uint32_t		Layer;
	FString			LayerName;
	FRushStateData	ActiveState;
	uint32_t		BlendingOutState;
	
private:
};