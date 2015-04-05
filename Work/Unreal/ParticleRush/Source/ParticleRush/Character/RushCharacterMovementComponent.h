// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "RushCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PARTICLERUSH_API URushCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:

	//Init
	virtual void InitializeComponent() override;

	//Tick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


#pragma region Movement States Setup - Walk
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Parameters"))
		float DefaultMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Parameters"))
		float DefaultMaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Parameters"))
		float DefaultDeceleration;
#pragma endregion


#pragma region Movement States Setup - Boost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Parameters"))
	float BoostSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Parameters"))
	float BoostAccelerationIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Parameters"))
	float BoostDecelerationDecrease;
#pragma endregion
};
