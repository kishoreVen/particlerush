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
		StateDuration = 0.0f;
		TransitionStateOnTimeOut = ERushState::None;

		_timeLeftToTransitionOut = -1.0f;
	}

	FRushStateData(uint32_t layerIndex, FString stateName, float stateDuration, uint32_t transitionStateOnTimeOut)
	{
		LayerIndex = layerIndex;
		StateName = stateName;
		StateDuration = stateDuration;
		TransitionStateOnTimeOut = transitionStateOnTimeOut;
	}

	uint32_t	LayerIndex;
	FString		StateName;
	float		StateDuration;
	uint32_t	TransitionStateOnTimeOut;

	bool Update(float DeltaSeconds)
	{
		if (FMath::IsNearlyEqual(_timeLeftToTransitionOut, -1.0f))
			return false;

		if (_timeLeftToTransitionOut < 0.0f)
		{
			_timeLeftToTransitionOut = -1.0f;
			return true;
		}

		_timeLeftToTransitionOut -= DeltaSeconds;

		return false;
	}

	void Enter()
	{
		if (FMath::IsNearlyEqual(StateDuration, 0.0f))
		{
			_timeLeftToTransitionOut = -1.0f;			
		}
		else
		{
			_timeLeftToTransitionOut = StateDuration;
		}
	}

private:
	float		_timeLeftToTransitionOut;
};