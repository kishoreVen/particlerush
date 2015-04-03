// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushStateManager.h"

RushStateManager::RushStateManager()
{
	InitializeAllStateData();

	// Set all layers to none as its active state
	for (uint32_t layer = 0; layer < ERushStateLayer::Everything; layer++)
	{
		_activeStates.Add(layer, ERushState::None);
	}
}

RushStateManager::~RushStateManager()
{
}


uint32_t RushStateManager::GetCurrentState(uint32_t layer)
{
	return (layer < ERushStateLayer::Everything) ? _activeStates[layer] : ERushState::None;
}

bool RushStateManager::RequestStateChange(uint32_t layer, uint32_t state)
{
	return SetCurrentState(layer, state);
}

bool RushStateManager::SetCurrentState(uint32_t layer, uint32_t state)
{
	if (layer < ERushStateLayer::Everything)
	{
		_activeStates[layer] = state;

		_stateCollection[state].Enter();

		return true;
	}

	return false;
}

void RushStateManager::InitializeAllStateData()
{
	for (uint32_t state = 0; state < ERushState::Everything; state++)
	{
		_stateCollection.Add(FRushStateData());
	}

	_stateCollection[ERushState::Boost] = FRushStateData(ERushStateLayer::Locomotion, "Boost", 2.0f, ERushState::Walk);
	_stateCollection[ERushState::Walk] = FRushStateData(ERushStateLayer::Locomotion, "Boost", 0.0f, ERushState::None);
}


void RushStateManager::UpdateCurrentStates(float DeltaSeconds)
{
	int32_t numStates = _stateCollection.Num();

	for (int32_t state = 0; state < numStates; state++)
	{
		if (_stateCollection[state].Update(DeltaSeconds))
		{
			RequestStateChange(_stateCollection[state].LayerIndex, _stateCollection[state].TransitionStateOnTimeOut);
		}
	}
}