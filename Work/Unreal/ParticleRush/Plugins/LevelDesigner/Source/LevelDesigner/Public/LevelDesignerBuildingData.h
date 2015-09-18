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
	UBlueprint* BuildingBlueprint;

	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Meta = (Category = "Spawn", DisplayName = "Default Building Mesh (If not set in BP)"))
	UStaticMesh* DefaultBuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (Category = "Spawn"))
	FColor BuildingColor;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params"))
	FVector MinDimensions;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params"))
	FVector MaxDimensions;
};