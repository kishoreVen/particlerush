// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCameraArmComponent.h"

/* Engine Headers*/

/* Custom Headers */
#include "Character/RushCharacter.h"

void URushCameraArmComponent::MaintainDistanceFromRush(float DeltaTime)
{
	ARushCharacter* rush = static_cast<ARushCharacter*>(GetOwner());

	if (rush == NULL)
		return;

	UCharacterMovementComponent* movementComponent = static_cast<UCharacterMovementComponent*>(rush->GetMovementComponent());

	float currentArmLength = this->TargetArmLength;
	float mappedSpeedOfRush = movementComponent->Velocity.Size();

	mappedSpeedOfRush = FMath::GetMappedRangeValue(FVector2D(0.0f, movementComponent->GetMaxSpeed()), SpeedImpactOnArmCatchup, mappedSpeedOfRush);

	float targetArmLength = DefaultDistanceFromRush + mappedSpeedOfRush;

	float interolatedArmLength = FMath::FInterpTo(currentArmLength, targetArmLength, DeltaTime, ArmCatchUpSpeed);
	this->TargetArmLength = interolatedArmLength;	
}


void URushCameraArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MaintainDistanceFromRush(DeltaTime);
}