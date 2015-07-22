// Particle Rush 2014 - 2015

#pragma once

#include "GameFramework/GameMode.h"
#include "ParticleRushGameMode.generated.h"

/**
 * The default main game mode
 */
UCLASS(BlueprintType)
class PARTICLERUSH_API AParticleRushGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AParticleRushGameMode(const class FObjectInitializer& ObjectInitalizer);
};
