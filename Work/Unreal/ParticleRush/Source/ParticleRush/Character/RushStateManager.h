// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Custom Headers */
#include "RushState.h"
#include "RushStateLayer.h"
#include "RushStateData.h"

/**
 * A management system of character states 
 */
class PARTICLERUSH_API RushStateManager
{
public:
	RushStateManager();
	~RushStateManager();

	uint32_t GetCurrentState(uint32_t layer);

	bool RequestStateChange(uint32_t layer, uint32_t state);

	void UpdateCurrentStates(float DeltaSeconds);

private:
	TArray<FRushStateData> _stateCollection;

	TMap<uint32_t, uint32_t> _activeStates;

	bool SetCurrentState(uint32_t layer, uint32_t state);

	void InitializeAllStateData();
};
