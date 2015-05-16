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
};
