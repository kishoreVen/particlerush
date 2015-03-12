// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCameraComponent.h"

/* Custom Headers */
#include "RushPawn.h"

void URushCameraComponent::LookAtPoint(FVector aWorldPoint)
{
	ARushPawn* rush = static_cast<ARushPawn*>(GetOwner());

	if (rush == NULL)
		return;

	
	FVector cameraLocation = this->GetComponentLocation();
	
	FVector lookAtDirection = aWorldPoint - cameraLocation;
	FRotator currentRotation = this->GetComponentRotation();
	FRotator lookAtRotation = lookAtDirection.Rotation();
	FRotator interpRotation = FMath::InterpEaseOut<FRotator>(currentRotation, lookAtRotation, LookAtEaseAlpha, LookAtEaseExp);
	
	this->SetWorldRotation(interpRotation);	
}


void URushCameraComponent::LookAtRush()
{
	ARushPawn* rush = static_cast<ARushPawn*>(GetOwner());

	if (rush == NULL)
		return;

	
	FVector rushLocation = rush->GetActorLocation();

	LookAtPoint(rushLocation);
}


void URushCameraComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LookAtRush();
}