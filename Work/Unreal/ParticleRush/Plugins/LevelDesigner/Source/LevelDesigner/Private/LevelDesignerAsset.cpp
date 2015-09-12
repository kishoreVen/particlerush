#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerAsset.h"

/*--------------------------
Level Designer Settings
---------------------------*/
ULevelDesignerAsset::ULevelDesignerAsset()
: DefaultAlleySpacing(50.0f)
, DefaultRotationalVariance(75.0f)
, DefaultBoxPopulationDensity(0.7f)
{
}

void ULevelDesignerAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ULevelDesignerAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if ((PropertyChangedEvent.Property != nullptr) && (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ULevelDesignerAsset, LevelDesignerBuildings)))
	{
		const int32 atIndex = PropertyChangedEvent.GetArrayIndex(PropertyChangedEvent.Property->GetFName().ToString());
		
		if (atIndex == INDEX_NONE)
			return;

		if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayAdd)
		{
			FColor buildingColor = GenerateUniqueBuildingClassColor();
			LevelDesignerBuildings[atIndex].BuildingColor = buildingColor;
		}
	}
}

FColor ULevelDesignerAsset::GenerateUniqueBuildingClassColor()
{
	static FColorList colorList;
	colorList.CreateColorMap();

	int maxColors = colorList.GetColorsNum();
	int colorIndex = FMath::RandRange(0, maxColors - 1);
	FColor generatedColor = colorList.GetFColorByIndex(colorIndex);

	bool isDuplicate = true;
	while (isDuplicate)
	{
		isDuplicate = false;
		for (int buildingIndex = 0; buildingIndex < LevelDesignerBuildings.Num(); buildingIndex++)
		{
			if (generatedColor == LevelDesignerBuildings[buildingIndex].BuildingColor)
			{
				isDuplicate = true;
				colorIndex = FMath::RandRange(0, maxColors - 1);
				generatedColor = colorList.GetFColorByIndex(colorIndex);

				break;
			}
		}
	}

	return generatedColor;
}

FLevelDesignerBuildingData ULevelDesignerAsset::GetRandomBuildingData()
{
	int randomClassIndex = FMath::RandRange(0, LevelDesignerBuildings.Num() - 1);

	return LevelDesignerBuildings[randomClassIndex];
}