// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "Obstacle.h"


// Sets default values
AObstacle::AObstacle(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	#pragma region Component Setup
	staticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp"));
	RootComponent = staticMeshComp;
	staticMeshComp->SetMobility(EComponentMobility::Static);
	#pragma endregion
	
	InitializeObstacle();
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}


void AObstacle::InitializeObstacle()
{
	
}