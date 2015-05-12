// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "Camera/CameraComponent.h"

/* Generated Headers */
#include "RushCameraComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API URushCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

#pragma region Behaviors
private:
	FRotator _targetRotation;

	float	 _blendTime;

protected:

public:
	void RotateCameraToStoredTarget(float DeltaTime);

	void RequestCameraStageSwitch(FRotator targetRotation, float BlendTime);
#pragma endregion
};
