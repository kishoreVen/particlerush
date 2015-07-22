// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Level/Obstacle/Obstacle.h"
#include "RefractObstacle.generated.h"

UCLASS()
class PARTICLERUSH_API ARefractObstacle : public AObstacle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Meta = (Category = "Refract Params"))
	float RefractiveIndex;

public:
	UFUNCTION(BlueprintCallable, Meta = (Category = "Refract Params"))
	float GetRefractiveIndex();

	UFUNCTION(BlueprintCallable, Meta = (Category = "Refract Params"))
	void RequestCollisionEnabledToggle(bool EnableCollision);
};

FORCEINLINE float ARefractObstacle::GetRefractiveIndex()
{
	return RefractiveIndex;
}
