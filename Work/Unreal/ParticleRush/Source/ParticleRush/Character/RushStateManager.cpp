// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushStateManager.h"

#include "RushStateMatrix.h"

RushStateManager::RushStateManager()
{
	InitializeAllStateData();

	InitialzeAllLayerData();
}

RushStateManager::~RushStateManager()
{
}


uint32_t RushStateManager::GetCurrentState(uint32_t layer)
{
	return (layer < ERushStateLayer::Everything) ? _layerCollection[layer].ActiveState.StateIndex : ERushState::None;
}

bool RushStateManager::RequestStateChange(uint32_t layer, uint32_t state)
{
	if (layer < ERushStateLayer::Everything)
	{
		if (_layerCollection[layer].RequestStateChange(state))
		{
			_layerCollection[layer].SetState(_stateCollection[state]);

			return true;
		}
	}

	return false;
}

void RushStateManager::InitializeAllStateData()
{
	_stateCollection.Empty();

	for (uint32_t state = ERushState::None; state < ERushState::Everything; state++)
	{
		_stateCollection.Add(FRushStateData());
	}

	_stateCollection[ERushState::Boost] = FRushStateData(ERushStateLayer::Locomotion, ERushState::Boost, "Boost", 2.0f, ERushState::Walk);
	_stateCollection[ERushState::Walk] = FRushStateData(ERushStateLayer::Locomotion, ERushState::Walk, "Walk", 0.0f, ERushState::None);
}

void RushStateManager::InitialzeAllLayerData()
{
	_layerCollection.Empty();

	for (uint32_t layer = ERushStateLayer::None; layer < ERushState::Everything; layer++)
	{
		_layerCollection.Add(FRushStateLayerData());
	}

	_layerCollection[ERushStateLayer::Locomotion] = FRushStateLayerData(ERushStateLayer::Locomotion, "Locomotion", _stateCollection[ERushState::Walk]);
}


void RushStateManager::UpdateCurrentStates(float DeltaSeconds)
{
	for (uint32_t layer = ERushStateLayer::None; layer < ERushState::Everything; layer++)
	{
		_layerCollection[layer].Update(DeltaSeconds);
	}
}