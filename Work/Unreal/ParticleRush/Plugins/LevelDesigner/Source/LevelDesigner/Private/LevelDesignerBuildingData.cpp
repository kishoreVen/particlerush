#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerBuildingData.h"


/*--------------------------
Level Designer Struct
---------------------------*/
FString FLevelDesignerBuildingData::DefaultAssetName("/Game/Assets/Prototype/DefaultBuildingMesh");

FLevelDesignerBuildingData::FLevelDesignerBuildingData()
{
	BuildingObject = StaticLoadObject(UStaticMesh::StaticClass(), NULL, *DefaultAssetName);
}

FLevelDesignerBuildingData::~FLevelDesignerBuildingData()
{ 
	for (int actorIndex = 0; actorIndex < ActorsSpawned.Num(); actorIndex++)
	{
		ActorsSpawned[actorIndex]->Destroy();
	}

	ActorsSpawned.Empty();
}

void FLevelDesignerBuildingData::AddSpawnedActor(AActor* spawnedActor)
{
	ActorsSpawned.Add(spawnedActor);
}

void FLevelDesignerBuildingData::RemoveSpawnedActor(AActor* removedActor)
{
	ActorsSpawned.Remove(removedActor);
}