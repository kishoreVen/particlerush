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
	/* Set the time of activation for boost and offset by the duration for the boost */
	float currentBoostTime = GetWorld()->GetTimeSeconds();
	_lastBoostActvationTime = currentBoostTime + RushData.BoostDuration;

	
	RushFlags.ChainBoostStage = FMath::Clamp(RushFlags.ChainBoostStage + 1, 1, RushData.MaxBoostStages);
	_timeLeftForBoostToEnd = RushData.BoostDuration;

	OnBoostStageUp(RushFlags.ChainBoostStage, ((float)RushFlags.ChainBoostStage / (float)RushData.MaxBoostStages));
}


void ARushCharacter::ExecuteBoostPerTick(float DeltaTime)
{
	if (RushFlags.ChainBoostStage <= 0)
		return;

	_timeLeftForBoostToEnd -= DeltaTime;

	if (_timeLeftForBoostToEnd < 0.0f)
	{
		/* Reset Boost stages if boost not activated for more than a certain time */
		float currentTime = GetWorld()->GetTimeSeconds();
		if (currentTime - _lastBoostActvationTime > RushData.BoostChainResetDuration)
		{
			RushFlags.ChainBoostStage = 0;
		}
		
		/* If Boost End hasn't already been declared, declare it */
		if (-1.0f != _timeLeftForBoostToEnd)
		{
			_timeLeftForBoostToEnd = -1.0f;
			OnBoostEnd();
		}
	}
}
