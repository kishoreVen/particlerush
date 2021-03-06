// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "AcceleratorTrackGenerator.h"

/* Engine Headers */
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AAcceleratorTrackGenerator::AAcceleratorTrackGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TrackSpline = CreateDefaultSubobject<USplineComponent>("TrackSpline");
	if (TrackSpline != NULL)
	{
		RootComponent = TrackSpline;
	}
}

// Called when the game starts or when spawned
void AAcceleratorTrackGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAcceleratorTrackGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


void AAcceleratorTrackGenerator::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}


#pragma region PROPERTIY ACCESSORS
FRotator AAcceleratorTrackGenerator::GetOnTrackDeltaRotation(float DeltaTime)
{
	return OnTrackRotationRotate * DeltaTime;
}
#pragma endregion


#pragma region METHODS
#pragma endregion