// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"
#include "Level/Obstacle/RefractObstacle.h"
#include "Generic/Utilities.h"

void ARushCharacter::InitializeBehaviorJump()
{
}


void ARushCharacter::StartJump()
{
	if (!IsInputDOFActive(EInputDOF::JUMP))
		return;

	if (RushMovementComponent != NULL && RushMovementComponent->IsMovingOnGround())
		RushMovementComponent->StartJump();
}


void ARushCharacter::StopJump()
{
	if (RushMovementComponent != NULL && RushMovementComponent->IsFalling())
		RushMovementComponent->StopJump();
}