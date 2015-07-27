#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerBuildingClass.h"

/*--------------------------
Level Designer Building Class
----------------------------*/

FLevelDesignerBuilding::FLevelDesignerBuilding()
: SpawnableBuildingPtr(NULL)
{
	
}

FLevelDesignerBuilding::~FLevelDesignerBuilding()
{
	
}

TSharedPtr<SLevelDesignerBuildingWidget> FLevelDesignerBuilding::MakeWidget()
{
	TSharedPtr<SLevelDesignerBuildingWidget> widget = SNew(SLevelDesignerBuildingWidget)
													  .BuildingClassColor(BuildingClassColor);
	
	return widget;
}


/*--------------------------
Level Designer Building Widget
----------------------------*/

SLevelDesignerBuildingWidget::SLevelDesignerBuildingWidget()
{
	
}

SLevelDesignerBuildingWidget::~SLevelDesignerBuildingWidget()
{
	
}

void SLevelDesignerBuildingWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNullWidget::NullWidget
	];
}