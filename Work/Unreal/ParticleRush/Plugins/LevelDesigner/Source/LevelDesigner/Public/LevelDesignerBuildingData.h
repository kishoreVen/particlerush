// Some copyright should be here...
#pragma once

#include "LevelDesignerBuildingData.generated.h"

USTRUCT(BlueprintType)
struct FLevelDesignerBuildingData
{
	GENERATED_USTRUCT_BODY()

private:
	static FString DefaultAssetName;

	TArray<TWeakObjectPtr<AActor>> ActorsSpawned;

public:
	FLevelDesignerBuildingData();
	~FLevelDesignerBuildingData();

	void AddSpawnedActor(AActor* spawnedActor);
	void RemoveSpawnedActor(AActor* removedActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Spawn"))
	UClass* BuildingClass;

	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Meta = (Category = "Spawn"))
	UObject* BuildingObject;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (Category = "Spawn"))
	FColor BuildingColor;
};