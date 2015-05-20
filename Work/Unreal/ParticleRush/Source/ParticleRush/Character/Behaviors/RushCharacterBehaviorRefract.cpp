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

#if !UE_BUILD_SHIPPING
	_shouldDrawWallCollisionResults = false;
#endif //!UE_BUILD_SHIPPING
}


bool ARushCharacter::RefractAgainstObstacle(class AActor* OtherActor, const FHitResult& HitResult)
{
	/* Check if Bounce Can happen Here */
	ARefractObstacle* collidedWall = dynamic_cast<ARefractObstacle*>(OtherActor);

	if (collidedWall == NULL)
		return false;

	_localRefractionCache = collidedWall;

	PerformRefraction(HitResult.Normal, collidedWall->GetRefractiveIndex());

#if !UE_BUILD_SHIPPING
	if (_shouldDrawWallCollisionResults)
	{
		DrawDebugDirectionalArrow(GetWorld(), HitResult.Location, HitResult.Location + HitResult.Normal * 50.0f, 5.0f, FColor::Yellow, false, 1.0f);
	}
#endif //!UE_BUILD_SHIPPING

	return true;
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

#if !UE_BUILD_SHIPPING
	if (_shouldDrawWallCollisionResults)
	{
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + refractDirection * 50.0f, 5.0f, FColor::Red, false, 1.0f);
	}
#endif //!UE_BUILD_SHIPPING
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