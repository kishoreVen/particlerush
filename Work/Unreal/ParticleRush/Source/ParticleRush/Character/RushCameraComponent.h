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

	FRotator _defaultRotation;

	FRotator _targetRightStickRotation;

	float	 _blendTime;

	bool	 _isSwitchingCamera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Control"))
	float CameraRightStickRotationBlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Control"))
	FRotator CameraRightStickRotationMaxAngle;	

public:
	void InitializeCamera();

	void RotateCameraToStoredTarget(float DeltaTime);

	void RotateCameraToRightStickTarget(float DeltaTime);

	void RequestCameraStageSwitch(FRotator targetRotation, float BlendTime);

	void SetYawForRightStickRotation(float value);

	void SetRollForRightStickRotation(float value);
#pragma endregion
};
