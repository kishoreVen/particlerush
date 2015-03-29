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

#pragma region Local Cache
private:
	float _rushLastFrameSpeedCache;
#pragma endregion
	
#pragma region Blueprint Param Declerations
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Distance Params"))
	float DefaultDistanceFromRush;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Camera Target Lag Params"))
	FVector2D SpeedImpactOnArmCatchup;
#pragma endregion

#pragma region Base Overrides
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
#pragma endregion

#pragma region Helper Methods
public:
	void DoCameraLag(float DeltaTime);
#pragma endregion
};
