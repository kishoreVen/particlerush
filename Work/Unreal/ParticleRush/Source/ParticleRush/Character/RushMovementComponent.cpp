// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushMovementComponent.h"


URushMovementComponent::URushMovementComponent()
{
	GravityScale = 1.0f;
}

float URushMovementComponent::GetGravityZ() const
{
	return Super::GetGravityZ() * GravityScale;
}