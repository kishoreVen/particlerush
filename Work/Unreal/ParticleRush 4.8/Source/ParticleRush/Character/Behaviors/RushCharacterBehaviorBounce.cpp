// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"
#include "Level/Obstacle/BounceObstacle.h"
#include "Generic/ParticleRushUtils.h"

void ARushCharacter::InitializeBehaviorBounce()
{
	_timeBeforeRegainingControlFromBounce = -1.0f;
	_bounceTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
}


bool ARushCharacter::BounceAgainstObstacle(class AActor* OtherActor, const FHitResult& HitResult)
{
	/* Check if Bounce Can happen Here */
	ABounceObstacle* collidedWall = dynamic_cast<ABounceObstacle*>(OtherActor);

	if (collidedWall == NULL)
		return false;

	PerformBounce(HitResult.Normal, collidedWall->GetBounceZOverrideFactor());

	return true;
}


void ARushCharacter::PerformBounce(FVector HitNormal, float OverrideZImpulseFactor)
{
	_timeBeforeRegainingControlFromBounce = RushData.BounceDuration;

	FVector rushHeading = GetActorForwardVector();
	rushHeading.Z = 0.0f;

	FVector normal = HitNormal;
	normal.Z = 0.0f;

	FVector bounceDirection = UParticleRushUtils::GetReflectionVector(rushHeading, normal);
	bounceDirection.Normalize();

	_bounceTargetOrientation = bounceDirection.Rotation();

	URushCharacterMovementComponent* movementComponent = static_cast<URushCharacterMovementComponent*>(GetCharacterMovement());
	FVector bounceImpulse = bounceDirection * RushData.BounceStrength.GetInterpolatedValue(RushFlags.MomentumPercentage);

	bounceImpulse.Z = RushData.BounceJumpStrength.GetInterpolatedValue(RushFlags.MomentumPercentage) * OverrideZImpulseFactor;

	movementComponent->AddImpulse(bounceImpulse, true);


	if (_shouldDrawWallCollisionResults)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, bounceImpulse.ToString());
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + bounceDirection * 50.0f, 5.0f, FColor::Red, false, 1.0f);
	}
}


void ARushCharacter::ExecuteBouncePerTick(float DeltaTime)
{
	if (_timeBeforeRegainingControlFromBounce == -1.0f)
	{
		return;
	}

	_timeBeforeRegainingControlFromBounce -= DeltaTime;

	if (_timeBeforeRegainingControlFromBounce < 0.0f)
	{
		_timeBeforeRegainingControlFromBounce = -1.0f;
		_bounceTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		FRotator interpRotation = FMath::RInterpTo(GetControllerRotation(), _bounceTargetOrientation, DeltaTime, RushData.BounceOrientationStrength);
		SetControllerRotation(interpRotation);
	}
}