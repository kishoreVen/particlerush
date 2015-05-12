// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCameraComponent.h"

/* Custom Headers */
#include "Character/RushCharacter.h"

void URushCameraComponent::RotateCameraToStoredTarget(float DeltaTime)
{
	FRotator currentRotation = RelativeRotation;
	FRotator interpRotation = FMath::RInterpTo(currentRotation, _targetRotation, DeltaTime, _blendTime);

	RelativeRotation = interpRotation;
}


void URushCameraComponent::RequestCameraStageSwitch(FRotator targetRotation, float blendTime)
{
	_targetRotation = targetRotation;

	_blendTime = blendTime;
}