// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCameraArmComponent.h"

/* Custom Headers */
#include "Character/RushPawn.h"

void URushCameraArmComponent::MaintainDistanceFromRush(float DeltaTime)
{
	ARushPawn* rush = static_cast<ARushPawn*>(GetOwner());

	if (rush == NULL)
		return;

	float currentArmLength = this->TargetArmLength;

	float targetArmLength = DefaultDistanceFromRush;

	float interolatedArmLength = FMath::FInterpTo(currentArmLength, targetArmLength, DeltaTime, 4.0f);
	this->TargetArmLength = interolatedArmLength;	
}


void URushCameraArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MaintainDistanceFromRush(DeltaTime);
}