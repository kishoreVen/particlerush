// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCharacterMovementComponent.h"

/* Custom Headers */
#include "RushCharacter.h"

void URushCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//~~~~~~~~~~~~~~~~~

	//UE_LOG //comp Init!
}

//Tick Comp
void URushCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ARushCharacter* rush = static_cast<ARushCharacter*>(GetOwner());

	if (rush == NULL)
		return;

	TEnumAsByte<ERushState::Type> characterState = ERushState::Air;

	switch (characterState)
	{
	case ERushState::Air:
		MaxWalkSpeed				= DefaultMaxSpeed;
		MaxAcceleration				= DefaultMaxAcceleration;
		BrakingDecelerationWalking	= DefaultDeceleration;
		break;
	case ERushState::Walk:
		MaxWalkSpeed				= BoostMaxSpeed;
		MaxAcceleration				= BoostMaxAcceleration;
		BrakingDecelerationWalking	= BoostDeceleration;
		break;
	}
}


