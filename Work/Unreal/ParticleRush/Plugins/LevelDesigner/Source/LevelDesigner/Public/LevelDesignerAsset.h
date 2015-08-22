// Some copyright should be here...
#pragma once

#include  "LevelDesignerBuildingData.h"

#include "LevelDesignerAsset.generated.h"

 /**  */
UCLASS(BlueprintType)
class ULevelDesignerAsset : public UObject
{
	GENERATED_BODY()

private:
	FColor GenerateUniqueBuildingClassColor();

protected:
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;

public:
	ULevelDesignerAsset();

	UFUNCTION(BlueprintCallable, Meta = (Category = "Spawn Classes"))
	FLevelDesignerBuildingData GetRandomBuildingData();

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params"))
	FVector DefaultMinDimensions;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params"))
	FVector DefaultMaxDimensions;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params"))
	float DefaultAlleySpacing;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params", UIMin = 0.0f, UIMax = 360.0f))
	float DefaultRotationalVariance;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Params", UIMin = 0.0f, UIMax = 1.0f))
	float DefaultBoxPopulationDensity;

	UPROPERTY(EditAnywhere, Meta = (Category = "Spawn Classes"))
	TArray<FLevelDesignerBuildingData> LevelDesignerBuildings;
};