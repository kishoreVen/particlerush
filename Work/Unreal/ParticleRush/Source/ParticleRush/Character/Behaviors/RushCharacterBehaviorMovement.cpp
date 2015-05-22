// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Character/RushCharacter.h"
#include "Character/RushCharacterMovementComponent.h"


void ARushCharacter::InitializeBehaviorMovement()
{
	_targetMeshTurningRollAngle = 0.0f;
}

void ARushCharacter::OnBeginPlayBehaviorMovement()
{
	_defaultMeshRotator = GetMesh()->RelativeRotation;
}


void ARushCharacter::MoveForward(float value)
{
	if (Controller == NULL || !IsInputDOFActive(EInputDOF::MOVE))
		return;
	
	FVector actorForward = GetActorForwardVector();
	AddMovementInput(actorForward, value);	
}


void ARushCharacter::TurnRight(float value)
{
	if (Controller == NULL || !IsInputDOFActive(EInputDOF::TURN))
		return;

	AddControllerYawInput(value);

	/* Mesh Rotation for smooth mesh movement */
	_targetMeshTurningRollAngle = value * RushData.MeshTurningMaxAngle;
}


void ARushCharacter::ApplyBraking(float value)
{
	if (!IsInputDOFActive(EInputDOF::BRAKE))
		return;

	URushCharacterMovementComponent* movementComponent = static_cast<URushCharacterMovementComponent*>(GetMovementComponent());

	if (movementComponent != NULL)
		movementComponent->ApplyBraking(value);
}


void ARushCharacter::ActivateSharpTurn(float value)
{
	if (!IsInputDOFActive(EInputDOF::SHARP_TURN))
		return;

	if (Controller != NULL && value != 0.0f && _sharpTurnTarget.IsNearlyZero())
	{
		FRotator sharpRotator = FRotator::ZeroRotator;
		sharpRotator.Yaw = value * RushData.SharpTurnYawReach.GetInterpolatedValue(1 - RushFlags.MomentumPercentage);
		_sharpTurnTarget = Controller->GetControlRotation() + sharpRotator;

		SetInputDOFState(EInputDOF::SHARP_TURN, false);
		SetInputDOFState(EInputDOF::TURN, false);
	}
}


void ARushCharacter::ExecuteMeshRotationPerTick(float DeltaTime)
{
	USkeletalMeshComponent* mesh = GetMesh();

	if (mesh == NULL)
		return;

	FRotator targetRotator(0.0f, 0.0f, _targetMeshTurningRollAngle);
	FRotator currentMeshRotator = mesh->RelativeRotation;
	FRotator currentRotator = currentMeshRotator - _defaultMeshRotator;

	FRotator DeltaRotation = FMath::RInterpTo(currentRotator, targetRotator, DeltaTime, RushData.MeshTurningSpeed);
	DeltaRotation.Yaw = _defaultMeshRotator.Yaw;

	if (!DeltaRotation.IsNearlyZero())
		mesh->SetRelativeRotation(DeltaRotation);
}


void ARushCharacter::ExecuteSharpTurnPerTick(float DeltaTime)
{
	if (Controller == NULL || _sharpTurnTarget.IsNearlyZero())
		return;

	FRotator currentControllerRotation = Controller->GetControlRotation();
	FRotator difference = _sharpTurnTarget - currentControllerRotation;

	if (difference.IsNearlyZero(0.001f)) // Adjusted threshold to get faster and accurate results
	{
		_sharpTurnTarget = FRotator::ZeroRotator;

		SetInputDOFState(EInputDOF::SHARP_TURN, true);
		SetInputDOFState(EInputDOF::TURN, true);
	}
	else
	{
		FRotator interpControllerRotation = FMath::RInterpTo(currentControllerRotation, _sharpTurnTarget, DeltaTime, RushData.SharpTurnStrength);
		Controller->SetControlRotation(interpControllerRotation);
	}
}