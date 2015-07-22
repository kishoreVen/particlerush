// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "PhotonNode.h"


// Sets default values
APhotonNode::APhotonNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APhotonNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhotonNode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}