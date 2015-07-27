#pragma once

class FLevelDesignerBuilding;
class SLevelDesignerBuildingWidget;


class FLevelDesignerBuilding
{
private:
	UClass*					SpawnableBuildingPtr;
	FColor					BuildingClassColor;

public:
	FLevelDesignerBuilding();
	~FLevelDesignerBuilding();

	TSharedPtr<SLevelDesignerBuildingWidget> MakeWidget();
};


class SLevelDesignerBuildingWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelDesignerBuildingWidget)
		: _BuildingClassColor(FColorList::Blue)
	{
	}
		SLATE_ARGUMENT(FColor, BuildingClassColor)
	SLATE_END_ARGS()
	
	SLevelDesignerBuildingWidget();
	~SLevelDesignerBuildingWidget();
	
	void Construct(const FArguments& InArgs);
};