// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "RushCameraComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API URushCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

#pragma region Blueprint Param Declerations
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Blend Params"))
	float LookAtEaseAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Blend Params"))
	float LookAtEaseExp;
#pragma endregion

#pragma region Base Overrides
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
#pragma endregion
	
#pragma region Helper Methods
protected:
	void LookAtRush();

public:
	void LookAtPoint(FVector worldPoint);	
#pragma endregion
};
