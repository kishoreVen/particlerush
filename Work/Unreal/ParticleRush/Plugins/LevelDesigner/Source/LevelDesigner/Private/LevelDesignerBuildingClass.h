#pragma once

class FLevelDesignerBuilding;
class SLevelDesignerBuildingWidget;

class FAssetThumbnail;
class FAssetThumbnailPool;

class FLevelDesignerBuilding
{
private:
	UObject*				SpawnableBuildingPtr;
	FColor					BuildingClassColor;

public:
	FLevelDesignerBuilding(const FColor& buildingClassColor);
	~FLevelDesignerBuilding();

	void SetBuildingObject(class UObject* NewAsset);
	class UObject* GetBuildingObject() const;

	TSharedRef<SLevelDesignerBuildingWidget> MakeWidget();

	TSharedRef<SColorBlock> MakeColorBlockWidget();
};


class SLevelDesignerBuildingWidget : public SCompoundWidget
{
private:
	TSharedPtr<FAssetThumbnail>			AssetThumbnail;
	TSharedPtr<FAssetThumbnailPool>		AssetThumbnailPool;

public:
	SLATE_BEGIN_ARGS(SLevelDesignerBuildingWidget)
	{
	}
	SLATE_END_ARGS()
	
	SLevelDesignerBuildingWidget();
	~SLevelDesignerBuildingWidget();
	
	void Construct(const FArguments& InArgs);

	/* Properties */
	void SetBuildingObject(class UObject* NewAsset);
	bool IsAssetAcceptableForDrop(const UObject* InObject) const;
};