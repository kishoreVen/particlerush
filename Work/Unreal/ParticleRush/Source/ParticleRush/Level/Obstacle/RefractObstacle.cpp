// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RefractObstacle.h"
#include "Generic/ParticleRushDefines.h"

void ARefractObstacle::RequestCollisionEnabledToggle(bool EnableCollision)
{
	if (!EnableCollision)
		staticMeshComp->SetCollisionProfileName(COLLISION_OVERLAP_ALL_DYNAMIC);
	else
		staticMeshComp->SetCollisionProfileName(COLLISION_BLOCK_ALL);
}