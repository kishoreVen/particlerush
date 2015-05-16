// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Eninge Headers */
#include "GameFramework/SpringArmComponent.h"

/* Custom Headers */
#include "Generic/DataStructs.h"
#include "Character/RushCameraComponent.h"

/* Generated Headers */
#include "RushCameraArmComponent.generated.h"

/**
 * Contains the Spring arm for managing camera
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API URushCameraArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

#pragma region PRIVATE MEMBERS
private:
	float _rushLastFrameSpeedCache;

	float _targetTargetArmLength;

	FRotator _targetTargetArmRotation;

	uint32 _currentCameraSwitchStage;

	URushCameraComponent* RushCamera;
	
	/* On every tick, performs rotational and catch up lag */
	void DoCameraLag(float DeltaTime);

	/* On every tick, move the camera arm according to the specified camera setting */
	void UpdateCameraToReachSwitchTarget(float DeltaTime);	
#pragma endregion

#pragma region PARAMS FOR BP
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Distance Params"))
	float DefaultDistanceFromRush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Target Lag Params"))
	FVector2D SpeedImpactOnArmCatchup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Switch - Blend Params"))
	TArray<FCameraDataVector> CameraSwitchTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Switch - Blend Params"))
	float CameraSwitchBlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Switch - Blend Params"))
	bool EnableCameraSwitching;
#pragma endregion

#pragma region OVERRIDE
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
#pragma endregion

#pragma region PUBLIC METHODS
public:
	void RequestCameraStageSwitch();

	void RequestCameraStageSwitch(int32 stage);	

	void TurnCameraYaw(float value);

	void TurnCameraRoll(float value);

	/* Initialize Component */
	void InitializeCameraArm(URushCameraComponent* rushCamera);

	UFUNCTION(Meta = (Category = "Camera Switch"))
	int32 GetCurrentCameraStage();
#pragma endregion
};


#pragma region INLINE
FORCEINLINE int32 URushCameraArmComponent::GetCurrentCameraStage()
{
	return _currentCameraSwitchStage;
}

FORCEINLINE void URushCameraArmComponent::TurnCameraYaw(float value)
{
	if (RushCamera != NULL)
		RushCamera->SetYawForRightStickRotation(value);
}


FORCEINLINE void URushCameraArmComponent::TurnCameraRoll(float value)
{
	if (RushCamera != NULL)
		RushCamera->SetRollForRightStickRotation(value);
}
#pragma endregion
