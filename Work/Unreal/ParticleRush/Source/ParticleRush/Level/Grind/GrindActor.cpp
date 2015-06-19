// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "GrindActor.h"
#include "Components/SplineComponent.h"

// Sets default values
AGrindActor::AGrindActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	#pragma region Component Setup
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetMobility(EComponentMobility::Static);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->AttachParent = RootComponent;
	#pragma endregion

}

// Called when the game starts or when spawned
void AGrindActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrindActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

