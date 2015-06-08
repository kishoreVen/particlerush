// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "GameFramework/CharacterMovementComponent.h"

/* Custom Headers */
#include "Generic/DataStructs.h"

/* Generated Headers */
#include "RushCharacterMovementComponent.generated.h"



/**
 * Movement Component for Rush Character
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

	//Property Update
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


#pragma region Movement States Setup - Walk
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Wallk"))
	float DefaultMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Walk"))
	float DefaultMaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Walk"))
	float DefaultDeceleration;
#pragma endregion


#pragma region Movement States Setup - Boost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Boost"))
	float BoostSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Boost"))
	float BoostAccelerationIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Boost"))
	float BoostDecelerationDecrease;
#pragma endregion


#pragma region Movement States - Jump Mechanism
private:
	bool	_isJumping;
	float	_jumpKeyHoldTime;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Movement Params - Jump"))
	float JumpMaxKeyHoldTime;

public:
	UFUNCTION(Meta = (Category = "Jump Mechanism"))
	void StartJump();

	UFUNCTION(Meta = (Category = "Jump Mechanism"))
	void StopJump();
#pragma endregion


#pragma region Movement States - Stop Mechanism
private:
	float	_currentBrakingGroundFriction;
	float	_currentBrakingDecelerationIncrease;

protected:
	UPROPERTY(EditAnywhere, Meta = (Category = "Movement Params - Stop"))
	FStrengthData GroundFrictionBrakingStrength;

	UPROPERTY(EditAnywhere, Meta = (Category = "Movement Params - Stop"))
	FStrengthData BrakingDecelerationIncreaseStrength;

public:
	UFUNCTION(Meta = (Category = "Stop Mechanism"))
	void ApplyBraking(float value);
#pragma endregion
};
