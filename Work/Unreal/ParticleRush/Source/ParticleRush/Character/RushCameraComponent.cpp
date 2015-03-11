// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCameraComponent.h"

#include "RushPawn.h"

void URushCameraComponent::LookAtPoint(FVector WorldPoint)
{
	ARushPawn* rush = static_cast<ARushPawn*>(GetOwner());

	if (rush != NULL)
	{
		FVector cameraLocation = this->GetComponentLocation();
		
		FVector lookAtDirection = WorldPoint - cameraLocation;
		FRotator lookAtRotation = lookAtDirection.Rotation();

		this->SetWorldRotation(lookAtRotation);
	}
}


void URushCameraComponent::LookAtRush()
{
	ARushPawn* rush = static_cast<ARushPawn*>(GetOwner());

	if (rush != NULL)
	{
		FVector rushLocation = rush->GetActorLocation();

		LookAtPoint(rushLocation);
	}
}

