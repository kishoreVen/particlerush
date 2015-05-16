// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"
#include "Level/Obstacle/RefractObstacle.h"
#include "Generic/Utilities.h"

void ARushCharacter::InitializeBehaviorJump()
{
	JumpMaxHoldTime = RushData.JumpMaxHoldTimeForHeight;
}


void ARushCharacter::StartJump()
{
	if (CanJump())
	{
		URushCharacterMovementComponent* movementComponent = static_cast<URushCharacterMovementComponent*>(GetMovementComponent());
		
		if (movementComponent != NULL)
			movementComponent->StartJump();
	}
}


void ARushCharacter::StopJump()
{
	URushCharacterMovementComponent* movementComponent = static_cast<URushCharacterMovementComponent*>(GetMovementComponent());

	if (movementComponent != NULL && movementComponent->IsFalling())
		movementComponent->StopJump();
}