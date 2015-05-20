// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCharacterMovementComponent.h"

/* Custom Headers */
#include "Character/RushCharacter.h"

void URushCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	_jumpKeyHoldTime = 0.0f;

	_currentBrakingGroundFriction = GroundFrictionBrakingStrength.MinValue;
	_currentBrakingDecelerationIncrease = 0.0f;
}


void URushCharacterMovementComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.MemberProperty->GetName() == "GroundFrictionBrakingStrength")
		GroundFrictionBrakingStrength.UpdateProperties();
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

#pragma region STOP UPDATE
	GroundFriction = _currentBrakingGroundFriction;
	BrakingDecelerationWalking += _currentBrakingDecelerationIncrease;
#pragma endregion

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
	float currentMomentumPercentage = FMath::Clamp(currentVelocity / DefaultMaxSpeed, 0.0f, 1.0f);

	rush->RushFlags.MomentumDiffPercentage = currentMomentumPercentage - rush->RushFlags.MomentumPercentage;
	rush->RushFlags.MomentumPercentage = currentMomentumPercentage;
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


void URushCharacterMovementComponent::ApplyBraking(float value)
{
	_currentBrakingGroundFriction = GroundFrictionBrakingStrength.GetInterpolatedValue(value);
	_currentBrakingDecelerationIncrease = BrakingDecelerationIncreaseStrength.GetInterpolatedValue(value);
}