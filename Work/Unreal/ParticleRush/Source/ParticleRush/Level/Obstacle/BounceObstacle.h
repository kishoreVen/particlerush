// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Level/Obstacle/Obstacle.h"
#include "BounceObstacle.generated.h"

UCLASS()
class PARTICLERUSH_API ABounceObstacle : public AObstacle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Meta = (Category = "Bounce Params"))
	float BounceJumpFactor;

public:
	UFUNCTION(BlueprintCallable, Meta = (Category = "Bounce Params"))
	float GetBounceJumpFactor();
};

FORCEINLINE float ABounceObstacle::GetBounceJumpFactor()
{
	return BounceJumpFactor;
}
