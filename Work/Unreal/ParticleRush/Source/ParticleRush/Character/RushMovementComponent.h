// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "RushMovementComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API URushMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

	/* Default Constructor */
	URushMovementComponent();

protected:	
	UPROPERTY(Category = "Movement Params", EditAnywhere, BlueprintReadWrite)
	float GravityScale;

	virtual float GetGravityZ() const override;
};
