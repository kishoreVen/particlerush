// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Eninge Headers */
#include "GameFramework/SpringArmComponent.h"

/* Custom Headers */
#include "Generic/DataStructs.h"

/* Generated Headers */
#include "RushCameraArmComponent.generated.h"

/**
 * Contains the Spring arm for managing camera
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API URushCameraArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

#pragma region Private Members
private:
	float _rushLastFrameSpeedCache;

	float _targetTargetArmLength;

	FRotator _targetTargetArmRotation;

	uint32 _currentCameraSwitchStage;

	class URushCameraComponent* RushCamera;
	
	/* On every tick, performs rotational and catch up lag */
	void DoCameraLag(float DeltaTime);

	/* On every tick, move the camera arm according to the specified camera setting */
	void UpdateCameraToReachSwitchTarget(float DeltaTime);
#pragma endregion

#pragma region Blueprint Param Declerations
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Distance Params"))
	float DefaultDistanceFromRush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Target Lag Params"))
	FVector2D SpeedImpactOnArmCatchup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Blend Params"))
	TArray<FCameraDataVector> CameraSwitchTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Blend Params"))
	float CameraSwitchBlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Blend Params"))
	bool EnableCameraSwitching;
#pragma endregion

#pragma region Base Overrides
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
#pragma endregion

#pragma region Helper Methods
public:
	void RequestCameraStageSwitch();

	void RequestCameraStageSwitch(int32 stage);

	void SetCamera(class URushCameraComponent* rushCamera);
#pragma endregion
};
