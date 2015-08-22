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
	TArray<AActor*>			SpawnedActors;

public:
	FLevelDesignerBuilding(const FColor& buildingClassColor);
	~FLevelDesignerBuilding();

	/* Building Class manipulators */
	void SetBuildingObject(class UObject* NewAsset);
	class UObject* GetBuildingObject() const;

	/* Widget Tools */
	TSharedRef<SLevelDesignerBuildingWidget> MakeWidget();
	TSharedRef<SColorBlock> MakeColorBlockWidget();

	bool IsBuildingColorEqualTo(FColor& testBuildingColor);

	void RemoveAssociatedActors();

	/* String Operations */
	FString ToString();
	static FLevelDesignerBuilding* FromString(FString& string);
	
	static UObject* DefaultBuildingObject;
};


class SLevelDesignerBuildingWidget : public SCompoundWidget
{
private:
	TSharedPtr<FAssetThumbnail>			AssetThumbnail;
	TSharedPtr<FAssetThumbnailPool>		AssetThumbnailPool;
	TSharedPtr<SComboButton>			ComboButton;

	FLevelDesignerBuilding*				LevelDesignerBuilding;

public:
	SLATE_BEGIN_ARGS(SLevelDesignerBuildingWidget)
	{
	}
	SLATE_END_ARGS()
	
	SLevelDesignerBuildingWidget();
	~SLevelDesignerBuildingWidget();
	
	void Construct(const FArguments& InArgs, FLevelDesignerBuilding* InLevelDesignerBuilding);

	/* Properties */
	void SetBuildingObject(class UObject* NewAsset);
	bool IsAssetAcceptableForDrop(const UObject* InObject) const;

	void CloseMenu();
	TSharedRef<SWidget> OnGetMenuContent();
	void OnAssetSelectedFromPicker(const FAssetData& InAssetData);
	bool OnShouldFilterAsset(const FAssetData& AssetData) const;

	/* Browse Button Events */
	void OnBrowseToAsset();
	bool CanBrowseToAsset();

	/* Use Button Events */
	void OnUseSelectedAsset();
	bool CanUseSelectedAsset();

	/* Clear Button Events */
	void OnClearAsset();
	bool CanUseClearButton();
};