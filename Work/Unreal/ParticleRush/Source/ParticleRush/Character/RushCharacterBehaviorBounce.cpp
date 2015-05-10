// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCharacter.h"
#include "Level/Obstacle/BounceObstacle.h"
#include "RushCharacterMovementComponent.h"

#include "Generic/Utilities.h"

void ARushCharacter::InitializeBehaviorBounce()
{
	_timeBeforeRegainingControlFromBounce = -1.0f;
	_bounceTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);

	_shouldDrawWallCollisionResults = false;
}


void ARushCharacter::BounceAgainstWall(class AActor* OtherActor, const FHitResult& HitResult)
{
	/* Check if Bounce Can happen Here */
	ABounceObstacle* collidedWall = dynamic_cast<ABounceObstacle*>(OtherActor);

	if (collidedWall == NULL)
		return;	

	PerformBounce(HitResult.Normal);
}


void ARushCharacter::PerformBounce(FVector HitNormal)
{
	_timeBeforeRegainingControlFromBounce = RushData.BounceDuration;

	FVector rushHeading = GetActorForwardVector();
	rushHeading.Z = 0.0f;

	FVector normal = HitNormal;
	float normalZ = normal.Z;
	normal.Z = 0.0f;

	FVector bounceDirection = ParticleRush::Utilities::GetReflectionVector(rushHeading, normal);
	bounceDirection.Normalize();

	_bounceTargetOrientation = bounceDirection.Rotation();

	URushCharacterMovementComponent* movementComponent = static_cast<URushCharacterMovementComponent*>(GetCharacterMovement());
	float jumpFactor = (movementComponent->Velocity.Size() / movementComponent->MaxWalkSpeed);
	float bounceFactor = RushData.BounceStrength.DataValue2 * jumpFactor;
	bounceDirection.Z = jumpFactor * RushData.BounceJumpFactor; //* normalZ;

	bounceFactor = FMath::Max<float>(bounceFactor, RushData.BounceStrength.DataValue1);

	movementComponent->AddImpulse(bounceDirection * bounceFactor, true);

	if (_shouldDrawWallCollisionResults)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, (bounceDirection * bounceFactor).ToString());
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + bounceDirection * 50.0f, 5.0f, FColor::Red, false, 1.0f);
	}
}


void ARushCharacter::ExecuteBouncePerTick(float deltaSeconds)
{
	if (_timeBeforeRegainingControlFromBounce == -1.0f)
	{
		return;
	}

	_timeBeforeRegainingControlFromBounce -= deltaSeconds;

	if (_timeBeforeRegainingControlFromBounce < 0.0f)
	{
		_timeBeforeRegainingControlFromBounce = -1.0f;
		_bounceTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		if (Controller != NULL)
		{
			FRotator interpRotation = FMath::RInterpTo(Controller->GetControlRotation(), _bounceTargetOrientation, deltaSeconds, RushData.BounceOrientationStrength);
			Controller->SetControlRotation(interpRotation);
		}
	}
}


void ARushCharacter::ToggleDrawWallCollisionResults()
{
	_shouldDrawWallCollisionResults = !_shouldDrawWallCollisionResults;
}