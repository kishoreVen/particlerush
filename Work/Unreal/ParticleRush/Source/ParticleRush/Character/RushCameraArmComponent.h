// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "RushCameraArmComponent.generated.h"

/**
 * Contains the Spring arm for managing camera
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API URushCameraArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
#pragma region Blueprint Param Declerations
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Distance Params"))
	float DefaultDistanceFromRush;	
#pragma endregion

#pragma region Base Overrides
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
#pragma endregion

#pragma region Helper Methods
public:
	void MaintainDistanceFromRush(float DeltaTime);
#pragma endregion
};
