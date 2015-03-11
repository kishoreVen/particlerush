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
	
public:
	void LookAtPoint(FVector worldPoint);

	void LookAtRush();
};
