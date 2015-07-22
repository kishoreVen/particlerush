// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"

/* Custom Headers */
#include "Character/RushCharacter.h"
#include "Character/RushCameraComponent.h"
#include "Character/RushCameraArmComponent.h"


void URushCameraComponent::InitializeCamera()
{
	_defaultRotation = RelativeRotation;
}



void URushCameraComponent::RotateCameraToStoredTarget(float DeltaTime)
{
	FRotator currentRotation = RelativeRotation;
	
	if ((currentRotation - _targetRotation).IsNearlyZero())
	{
		_isSwitchingCamera = false;
		return;
	}

	FRotator interpRotation = FMath::RInterpTo(currentRotation, _targetRotation, DeltaTime, _blendTime);

	SetRelativeRotation(interpRotation);
}


void URushCameraComponent::RotateCameraToRightStickTarget(float DeltaTime)
{
	URushCameraArmComponent* cameraArm = static_cast<URushCameraArmComponent*>(AttachParent);
	if (cameraArm == NULL || !_isSwitchingCamera || (cameraArm->GetCurrentCameraStage() != 0 && cameraArm->GetCurrentCameraStage() != 1))
		return;

	FRotator currentRotation = RelativeRotation;

	FRotator interpRotation = FMath::RInterpTo(currentRotation, _targetRotation, DeltaTime, _blendTime);
	SetRelativeRotation(interpRotation);
}


void URushCameraComponent::RequestCameraStageSwitch(FRotator targetRotation, float blendTime)
{
	_targetRotation = targetRotation;
	_targetRightStickRotation = FRotator::ZeroRotator;

	_blendTime = blendTime;

	_isSwitchingCamera = true;
}


void URushCameraComponent::SetYawForRightStickRotation(float value)
{
	_targetRotation.Yaw = _defaultRotation.Yaw + value * CameraRightStickRotationMaxAngle.Yaw;
}


void URushCameraComponent::SetRollForRightStickRotation(float value)
{
	_targetRotation.Pitch = _defaultRotation.Pitch + value * CameraRightStickRotationMaxAngle.Pitch;
}