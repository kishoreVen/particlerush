// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "BarrierGenerator.h"

/* Engine Headers */
#include "Components/SplineComponent.h"


// Sets default values
ABarrierGenerator::ABarrierGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrierBase = CreateDefaultSubobject<USceneComponent>("BarrierBase");
	if (BarrierBase != NULL)
	{
		RootComponent = BarrierBase;
	}

	BarrierStart = CreateDefaultSubobject<UStaticMeshComponent>("BarrierStart");
	if (BarrierStart != NULL)
	{
		BarrierStart->AttachTo(BarrierBase, NAME_None, EAttachLocation::KeepRelativeOffset, true);
	}
}


// Called when the game starts or when spawned
void ABarrierGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABarrierGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


void ABarrierGenerator::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}