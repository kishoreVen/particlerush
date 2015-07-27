#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerBuildingClass.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetThumbnail.h"
#include "SAssetDropTarget.h"

/*--------------------------
Level Designer Building Class
----------------------------*/

FLevelDesignerBuilding::FLevelDesignerBuilding(const FColor& buildingClassColor)
: SpawnableBuildingPtr(NULL)
, BuildingClassColor(buildingClassColor)
{
	
}

FLevelDesignerBuilding::~FLevelDesignerBuilding()
{
	
}

TSharedRef<SLevelDesignerBuildingWidget> FLevelDesignerBuilding::MakeWidget()
{
	return SNew(SLevelDesignerBuildingWidget);
}

TSharedRef<SColorBlock> FLevelDesignerBuilding::MakeColorBlockWidget()
{
	static const FVector2D ColorBlockSize(32.0f, 32.0f);

	TSharedRef<SColorBlock> widget = SNew(SColorBlock)
									.Color(BuildingClassColor)
									.Size(ColorBlockSize);

	return widget;
}

void FLevelDesignerBuilding::SetBuildingObject(UObject* NewAsset)
{
	SpawnableBuildingPtr = NewAsset;
}

UObject* FLevelDesignerBuilding::GetBuildingObject() const
{
	return SpawnableBuildingPtr;
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
	static const FVector2D ThumbnailSize(64.0f, 64.0f);

	const UObject* BuildingObject = NULL;

	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(1024, true));
	AssetThumbnail = MakeShareable(new FAssetThumbnail(BuildingObject, ThumbnailSize.X, ThumbnailSize.Y, AssetThumbnailPool));

	TSharedRef<SWidget> AssetWidget =
		SNew(SAssetDropTarget)
		.OnIsAssetAcceptableForDrop(this, &SLevelDesignerBuildingWidget::IsAssetAcceptableForDrop)
		.OnAssetDropped(this, &SLevelDesignerBuildingWidget::SetBuildingObject)
		[
			SNew(SBox)
			.WidthOverride(ThumbnailSize.X)
			.HeightOverride(ThumbnailSize.Y)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(5)
			[
				AssetThumbnail->MakeThumbnailWidget()
			]
		];

	ChildSlot
	[
		AssetWidget
	];
}

void SLevelDesignerBuildingWidget::SetBuildingObject(UObject* NewAsset)
{
	if (UStaticMesh* NewStaticMesh = Cast<UStaticMesh>(NewAsset))
	{
		AssetThumbnail->SetAsset(NewAsset);
	}
	else if (AActor* BlueprintAsset = Cast<AActor>(NewAsset))
	{
		AssetThumbnail->SetAsset(NewAsset);
	}
}

bool SLevelDesignerBuildingWidget::IsAssetAcceptableForDrop(const UObject* InObject) const
{
	return (InObject->IsA(UStaticMesh::StaticClass()) || InObject->IsA(AActor::StaticClass()));
}