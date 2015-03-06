// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "GameFramework/Actor.h"

/* Custom Headers */

/* Generated Headers */
#include "Wall.generated.h"

/**
 * Class that represents each wall object in the level
 * Needed to simulate collision based behavior for each tile.
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API AWall : public AActor
{
	GENERATED_BODY()

#pragma region Base Class Overrides
#pragma endregion
};
