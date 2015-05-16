// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCharacterMovementComponent.h"

/* Custom Headers */
#include "RushCharacter.h"

void URushCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	_jumpKeyHoldTime = 0.0f;
}


void URushCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ARushCharacter* rush = static_cast<ARushCharacter*>(GetOwner());

	if (rush == NULL)
		return;

	MaxWalkSpeed = DefaultMaxSpeed;
	MaxAcceleration = DefaultMaxAcceleration;
	BrakingDecelerationWalking = DefaultDeceleration;

	int32 boostStage = rush->RushFlags.ChainBoostStage;

	if (boostStage > 0 && boostStage < rush->RushData.MaxBoostStages)
	{
		MaxWalkSpeed = DefaultMaxSpeed + BoostSpeedIncrease * boostStage;
		MaxAcceleration = DefaultMaxAcceleration + BoostAccelerationIncrease * boostStage;
		BrakingDecelerationWalking = DefaultDeceleration - BoostDecelerationDecrease * boostStage;
	}

#pragma region JUMP UPDATE
	if (_isJumping && _jumpKeyHoldTime < JumpMaxKeyHoldTime)
	{
		_jumpKeyHoldTime += DeltaTime;
		Velocity.Z = JumpZVelocity;
	}
	else
		_isJumping = false;
#pragma endregion

#pragma region UPDATE FLAGS
	float currentVelocity = Velocity.Size();
	rush->RushFlags.MomentumPercentage = FMath::Clamp(currentVelocity / DefaultMaxSpeed, 0.0f, 1.0f);
#pragma endregion
}


void URushCharacterMovementComponent::StartJump()
{
	_isJumping = true;
	_jumpKeyHoldTime = 0.0f;
	SetMovementMode(MOVE_Falling);
}


void URushCharacterMovementComponent::StopJump()
{
	_isJumping = false;
	_jumpKeyHoldTime = 0.0f;
}