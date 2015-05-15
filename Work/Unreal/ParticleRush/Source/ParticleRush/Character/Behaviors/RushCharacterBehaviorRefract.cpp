// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"
#include "Level/Obstacle/RefractObstacle.h"
#include "Generic/Utilities.h"

void ARushCharacter::InitializeBehaviorRefraction()
{
	_timeBeforeRegainingControlFromRefraction = -1.0f;
	_refractTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);

	_shouldDrawWallCollisionResults = false;
}


void ARushCharacter::RefractAgainstObstacle(class AActor* OtherActor, const FHitResult& HitResult)
{
	/* Check if Bounce Can happen Here */
	ARefractObstacle* collidedWall = dynamic_cast<ARefractObstacle*>(OtherActor);

	if (collidedWall == NULL)
		return;	

	_localRefractionCache = collidedWall;

	PerformRefraction(HitResult.Normal, collidedWall->GetRefractiveIndex());
}


void ARushCharacter::PerformRefraction(FVector HitNormal, float RefractiveIndex)
{
	FVector rushHeading = GetActorForwardVector();	

	FVector normal = HitNormal;	

	FVector refractDirection = ParticleRush::Utilities::GetRefractionVector(rushHeading, normal, RefractiveIndex);
	refractDirection.Normalize();

	if (!refractDirection.IsZero())
	{
		_timeBeforeRegainingControlFromRefraction = RushData.RefractDuration;
		_refractTargetOrientation = refractDirection.Rotation();

		_localRefractionCache->RequestCollisionEnabledToggle(false);
	}
}


void ARushCharacter::ExecuteRefractionPerTick(float deltaSeconds)
{
	if (_timeBeforeRegainingControlFromRefraction == -1.0f)
	{
		return;
	}

	if (_localRefractionCache != NULL)
	{
		if (!IsOverlappingActor(_localRefractionCache))
		{
			_localRefractionCache->RequestCollisionEnabledToggle(true);
			_localRefractionCache = NULL;
		}
	}

	_timeBeforeRegainingControlFromRefraction -= deltaSeconds;

	FRotator currentOrientation = Controller->GetControlRotation();

	if ((_timeBeforeRegainingControlFromRefraction < 0.0f) || 
		((currentOrientation - _refractTargetOrientation).IsNearlyZero())
		)
	{
		_timeBeforeRegainingControlFromRefraction = -1.0f;
		_refractTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		if (Controller != NULL)
		{
			FRotator interpRotation = FMath::RInterpTo(currentOrientation, _refractTargetOrientation, deltaSeconds, RushData.RefractOrientationStrength);
			Controller->SetControlRotation(interpRotation);
		}
	}
}