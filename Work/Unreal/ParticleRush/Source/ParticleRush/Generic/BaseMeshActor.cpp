// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "BaseMeshActor.h"

// Sets default values
ABaseMeshActor::ABaseMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	#pragma region Component Setup
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetMobility(EComponentMobility::Static);
	#pragma endregion
}


UStaticMesh* ABaseMeshActor::GetStaticMesh()
{
	return StaticMeshComp->StaticMesh;
}

UStaticMeshComponent* ABaseMeshActor::GetStaticMeshComponent()
{
	return StaticMeshComp;
}


void ABaseMeshActor::SetStaticMesh(UStaticMesh* NewStaticMesh)
{
	if (StaticMeshComp == NULL)
		return;

	TEnumAsByte<EComponentMobility::Type> prevMobility;

	/* Hack to fix mesh not being changeable with Static Components */
	if (StaticMeshComp->Mobility == EComponentMobility::Static || StaticMeshComp->Mobility == EComponentMobility::Stationary)
	{
		prevMobility = StaticMeshComp->Mobility;
		StaticMeshComp->SetMobility(EComponentMobility::Movable);
	}

	StaticMeshComp->SetStaticMesh(NewStaticMesh);

	if (prevMobility != StaticMeshComp->Mobility)
		StaticMeshComp->SetMobility(prevMobility);
}