// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"
#include "Generic/Utilities.h"


void ARushCharacter::InitializeBehaviorBoost()
{
	_timeLeftForBoostToEnd = -1.0f;	
}


void ARushCharacter::ActivateBoost()
{
	if (!IsInputDOFActive(EInputDOF::BOOST) 
		|| _timeLeftForBoostToEnd > 0.0f 
		|| RushFlags.MomentumPercentage < RushData.BoostMomentumThreshold)
		return;

	PerformBoost();
}


void ARushCharacter::PerformBoost()
{
	float currentBoostTime = GetWorld()->GetTimeSeconds();

	if (RushFlags.ChainBoostStage > 0)
	{
		if (currentBoostTime - _lastBoostActvationTime > RushData.BoostChainResetDuration)
		{
			RushFlags.ChainBoostStage = 0;
		}
	}

	_lastBoostActvationTime = currentBoostTime + RushData.BoostDuration;

	RushFlags.ChainBoostStage = (RushFlags.ChainBoostStage + 1) % RushData.MaxBoostStages;
	_timeLeftForBoostToEnd = RushData.BoostDuration;

	OnBoostStageUp(RushFlags.ChainBoostStage);
}


void ARushCharacter::ExecuteBoostPerTick(float DeltaTime)
{
	if (RushFlags.ChainBoostStage <= 0)
		return;

	_timeLeftForBoostToEnd -= DeltaTime;

	if (_timeLeftForBoostToEnd < 0.0f)
	{
		_timeLeftForBoostToEnd = -1.0f;

		OnBoostEnd();

		float currentBoostTime = GetWorld()->GetTimeSeconds();

		if (currentBoostTime - _lastBoostActvationTime > RushData.BoostChainResetDuration)
		{
			RushFlags.ChainBoostStage = 0;
		}
	}
}
