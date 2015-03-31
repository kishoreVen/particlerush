// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "BounceObstacle.h"


// Sets default values
ABounceObstacle::ABounceObstacle(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Component Setup
	staticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp"));
	RootComponent = staticMeshComp;
	staticMeshComp->SetMobility(EComponentMobility::Static);
	#pragma endregion
}

// Called when the game starts or when spawned
void ABounceObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABounceObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

