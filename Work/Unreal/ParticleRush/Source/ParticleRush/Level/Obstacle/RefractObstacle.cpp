// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RefractObstacle.h"
#include "Generic/ParticleRushDefines.h"

void ARefractObstacle::RequestCollisionEnabledToggle(bool EnableCollision)
{
	static FName NAME_DisableProfile		= COLLISION_OVERLAP_ALL_DYNAMIC;
	static FName NAME_EnableProfile			= COLLISION_BLOCK_ALL;

	if (!EnableCollision)
		staticMeshComp->SetCollisionProfileName(NAME_DisableProfile);
	else
		staticMeshComp->SetCollisionProfileName(NAME_EnableProfile);
}