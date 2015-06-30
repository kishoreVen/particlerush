#pragma once

#include "PropertyHandle.h"
#include "AssetThumbnail.h"

class BuildingParamsWidget : public SCompoundWidget
{
private:
	const FName DefaultBuildingMesh;

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

	/* Set Asset for DropTarget, Selection using ComboButton and Use Button asset selection*/
	void SetBuildingMesh(class UObject* NewAsset);

	/* Selected asset name and tooltip */
	FText GetSelectedAssetName() const;
	FText GetSelectedAssetPath() const;

	/* Function to Fetch the default static mesh.*/
	UStaticMesh* LoadMeshFromPath(const FName& Path);

public:
	TWeakObjectPtr<class UStaticMesh>	BuildingMeshPtr;
	TWeakObjectPtr<class UStaticMesh>	DefaultBuildingMeshPtr;
	FColor								BSPBuildingColor;

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
};

/* Inline Methods */
FORCEINLINE UStaticMesh* BuildingParamsWidget::LoadMeshFromPath(const FName& Path)
{
	if (Path == NAME_None) return NULL;

	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *Path.ToString()));
}