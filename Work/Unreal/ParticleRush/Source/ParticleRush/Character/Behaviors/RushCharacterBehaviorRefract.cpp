// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"
#include "Level/Obstacle/RefractObstacle.h"
#include "Generic/ParticleRushUtils.h"

void ARushCharacter::InitializeBehaviorRefraction()
{
	_timeBeforeRegainingControlFromRefraction = -1.0f;
	_refractTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
}


bool ARushCharacter::RefractAgainstObstacle(class AActor* OtherActor, const FHitResult& HitResult)
{
	/* Check if Bounce Can happen Here */
	ARefractObstacle* collidedWall = dynamic_cast<ARefractObstacle*>(OtherActor);

	if (collidedWall == NULL)
		return false;

	_localRefractionCache = collidedWall;

	PerformRefraction(HitResult.Normal, collidedWall->GetRefractiveIndex());


	if (_shouldDrawWallCollisionResults)
	{
		DrawDebugDirectionalArrow(GetWorld(), HitResult.Location, HitResult.Location + HitResult.Normal * 50.0f, 5.0f, FColor::Yellow, false, 1.0f);
	}

	return true;
}


void ARushCharacter::PerformRefraction(FVector HitNormal, float RefractiveIndex)
{
	FVector rushHeading = GetActorForwardVector();

	FVector normal = HitNormal;	

	FVector refractDirection = UParticleRushUtils::GetRefractionVector(rushHeading, normal, RefractiveIndex);
	refractDirection.Normalize();

	if (!refractDirection.IsZero())
	{
		_timeBeforeRegainingControlFromRefraction = RushData.RefractDuration;
		_refractTargetOrientation = refractDirection.Rotation();

		_localRefractionCache->RequestCollisionEnabledToggle(false);
	}


	if (_shouldDrawWallCollisionResults)
	{
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + refractDirection * 50.0f, 5.0f, FColor::Red, false, 1.0f);
	}
}


void ARushCharacter::ExecuteRefractionPerTick(float DeltaTime)
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

	_timeBeforeRegainingControlFromRefraction -= DeltaTime;

	FRotator currentOrientation = GetControllerRotation();

	if ((_timeBeforeRegainingControlFromRefraction < 0.0f) || 
		((currentOrientation - _refractTargetOrientation).IsNearlyZero())
		)
	{
		_timeBeforeRegainingControlFromRefraction = -1.0f;
		_refractTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		FRotator interpRotation = FMath::RInterpTo(currentOrientation, _refractTargetOrientation, DeltaTime, RushData.RefractOrientationStrength);
		SetControllerRotation(interpRotation);
	}
}