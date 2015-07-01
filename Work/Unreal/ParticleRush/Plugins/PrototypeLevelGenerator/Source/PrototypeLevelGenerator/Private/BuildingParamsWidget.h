#pragma once

#include "PropertyHandle.h"
#include "AssetThumbnail.h"

class BuildingParamsWidget : public SCompoundWidget
{
private:
	const FName DefaultBuildingMeshName;

	TSharedPtr<IPropertyHandle>			StaticMeshPropertyHandle;
	TSharedPtr<FAssetThumbnail>			AssetThumbnail;
	TSharedPtr<FAssetThumbnailPool>		AssetThumbnailPool;
	TSharedPtr<SComboButton>			ComboButton;

	/* Events for populating and using combo button */
	FText OnGetToolTip() const;
	void CloseMenu();
	TSharedRef<SWidget> OnGetMenuContent();
	void OnAssetSelectedFromPicker(const FAssetData& InAssetData);

	/* Drop Target Events */
	bool IsAssetAcceptableForDrop(const UObject* InObject) const { return InObject->IsA(UStaticMesh::StaticClass()); }

	/* Browse Button Events */
	void OnBrowseToStaticMesh();
	bool CanBrowseToAsset();

	/* Use Button Events */
	void OnUseSelectedStaticMesh();
	bool CanUseSelectedAsset();

	/* Clear Button Events */
	void OnClearStaticMesh();
	bool CanUseClearButton();

	/* Clear Dimensions Button Events */
	void OnClearDimensionsMin();
	bool CanUseClearDimesnionsMinButton();

	void OnClearDimensionsMax();
	bool CanUseClearDimesnionsMaxButton();

	/* Set Asset for DropTarget, Selection using ComboButton and Use Button asset selection*/
	void SetBuildingMesh(class UObject* NewAsset);

	/* Building Dimension Events */
	/*--Min--*/
	void SetMinBuildingDimensionsX(float NewScaleX, ETextCommit::Type TextCommitType) { MinDimension.X = NewScaleX; }
	void SetMinBuildingDimensionsY(float NewScaleY, ETextCommit::Type TextCommitType) { MinDimension.Y = NewScaleY; }
	void SetMinBuildingDimensionsZ(float NewScaleZ, ETextCommit::Type TextCommitType) { MinDimension.Z = NewScaleZ; }
	TOptional<float> GetMinBuildingDimensionsX() const { return MinDimension.X; }
	TOptional<float> GetMinBuildingDimensionsY() const { return MinDimension.Y; }
	TOptional<float> GetMinBuildingDimensionsZ() const { return MinDimension.Z; }
	/*--Max--*/
	void SetMaxBuildingDimensionsX(float NewScaleX, ETextCommit::Type TextCommitType) { MaxDimension.X = NewScaleX; }
	void SetMaxBuildingDimensionsY(float NewScaleY, ETextCommit::Type TextCommitType) { MaxDimension.Y = NewScaleY; }
	void SetMaxBuildingDimensionsZ(float NewScaleZ, ETextCommit::Type TextCommitType) { MaxDimension.Z = NewScaleZ; }
	TOptional<float> GetMaxBuildingDimensionsX() const { return MaxDimension.X; }
	TOptional<float> GetMaxBuildingDimensionsY() const { return MaxDimension.Y; }
	TOptional<float> GetMaxBuildingDimensionsZ() const { return MaxDimension.Z; }

	/* Selected asset name and tooltip */
	FText GetSelectedAssetName() const;
	FText GetSelectedAssetPath() const;

	/* Function to Fetch the default static mesh.*/
	UStaticMesh* LoadMeshFromPath(const FName& Path);

public:
	TWeakObjectPtr<UStaticMesh>								BuildingMeshPtr;
	static TWeakObjectPtr<UStaticMesh>						DefaultBuildingMeshPtr;
	FColor													BSPBuildingColor;
	static FVector											DefaultMinDimension;
	static FVector											DefaultMaxDimension;
	FVector													MinDimension;
	FVector													MaxDimension;

	BuildingParamsWidget();
	~BuildingParamsWidget();

	SLATE_BEGIN_ARGS(BuildingParamsWidget)
		: _BuildingColor(FColorList::Blue)
		, _AreRealTimeThumbnailsAllowed(true)
	{

	}
		SLATE_ARGUMENT(FColor, BuildingColor)
		SLATE_ATTRIBUTE(bool, AreRealTimeThumbnailsAllowed)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/* Public Interface to Identify Set mesh vs default mesh */
	bool IsDefaultMesh();

	/* Public Interface to Get Mesh Dimensions */
	FVector GetMinDimension();
	FVector GetMaxDimension();

	/* Interface to Min Dimension */
	void UpdateDefaultDimensions(FVector NewMinDimension, FVector NewMaxDimension);
};

/* Inline Methods */
FORCEINLINE UStaticMesh* BuildingParamsWidget::LoadMeshFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;

	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *Path.ToString()));
}