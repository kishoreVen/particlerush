// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Level/Obstacle/Obstacle.h"
#include "BounceObstacle.generated.h"

UCLASS()
class PARTICLERUSH_API ABounceObstacle : public AObstacle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Meta = (Category = "Bounce Params", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float BounceZOverrideFactor;

public:
	UFUNCTION(BlueprintCallable, Meta = (Category = "Bounce Params"))
	float GetBounceZOverrideFactor();
};

FORCEINLINE float ABounceObstacle::GetBounceZOverrideFactor()
{
	return BounceZOverrideFactor;
}
