// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCharacterMovementComponent.h"

/* Custom Headers */
#include "RushCharacter.h"

void URushCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
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

	/* Update Momentum Percentage */
	float currentVelocity = Velocity.Size();
	rush->RushFlags.MomentumPercentage = FMath::Clamp(currentVelocity / DefaultMaxSpeed, 0.0f, 1.0f);
}