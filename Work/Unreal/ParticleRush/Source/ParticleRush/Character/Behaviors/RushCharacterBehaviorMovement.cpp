// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"


void ARushCharacter::InitializeBehaviorMovement()
{
	_targetMeshTurningRollAngle = 0.0f;
}

void ARushCharacter::OnBeginPlayBehaviorMovement()
{
	_defaultMeshRotator = GetMesh()->RelativeRotation;
}

#pragma region Rush Input
void ARushCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		FVector actorForward = GetActorForwardVector();
		AddMovementInput(actorForward, value);
	}
}


void ARushCharacter::TurnRight(float value)
{
	if (Controller != NULL)
	{
		AddControllerYawInput(value);
	}	

	/* Mesh Rotation for smooth mesh movement */
	_targetMeshTurningRollAngle = value * RushData.MeshTurningMaxAngle;
}


void ARushCharacter::ExecuteMeshRotationPerTick(float deltaSeconds)
{
	USkeletalMeshComponent* mesh = GetMesh();

	if (mesh == NULL)
		return;

	FRotator targetRotator(0.0f, 0.0f, _targetMeshTurningRollAngle);
	FRotator currentMeshRotator = mesh->RelativeRotation;
	FRotator currentRotator = currentMeshRotator - _defaultMeshRotator;

	FRotator DeltaRotation = FMath::RInterpTo(currentRotator, targetRotator, deltaSeconds, RushData.MeshTurningSpeed);
	DeltaRotation.Yaw = _defaultMeshRotator.Yaw;

	if (!DeltaRotation.IsNearlyZero())
		mesh->SetRelativeRotation(DeltaRotation);
}