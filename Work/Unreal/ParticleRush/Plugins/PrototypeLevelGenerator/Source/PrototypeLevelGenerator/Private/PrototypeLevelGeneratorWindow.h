#pragma once

#include "BuildingParamsWidget.h"

class PrototypeLevelGeneratorWindow : public SCompoundWidget
{
private:
	int32		mMapSizeX;
	int32		mMapSizeY;
	int32		mNumBuildingClasses;

	FVector		mMinBuildingDimensions;
	FVector		mMaxBuildingDimensions;

	float		mAlleySpacing;

	TArray<TSharedPtr<BuildingParamsWidget>> BuildingMeshes;

	TSharedPtr<SVerticalBox> BuildingClassesVerticalSlot;

	/* Construction Helpers */
	void UpdateBuildingClassesSlot();

	class UWorld* GetWorld();

	void UpdateBuildingParams(FVector NewMinDimension, FVector NewMaxDimension);

	/* Generate Button Click Events and Helpers */
	FReply GeneratePrototypeLevelClicked();	
	void GenerateLevelObject(TSharedPtr<BuildingParamsWidget> buildingParams, FVector position);

	/* Accessors */
	/* Map Size Events */
	void SetMapSizeX(int32 value) { mMapSizeX = value; }
	TOptional<int32> GetMapSizeX() const { return mMapSizeX; }
	void SetMapSizeY(int32 value) { mMapSizeY = value; }
	TOptional<int32> GetMapSizeY() const { return mMapSizeY; }

	/* Alley Spacing Events */
	void SetAlleySpacing(float value) { mAlleySpacing = value; }
	TOptional<float> GetAlleySpacing() const { return mAlleySpacing; }

	/* Num Building Classes Events */
	void SetNumBuildingClasses(int32 value) { mNumBuildingClasses = value; UpdateBuildingClassesSlot(); }
	TOptional<int32> GetNumBuildingClasses() const { return mNumBuildingClasses; }

	/* Building Dimension Events */
	/*--Min--*/
	void SetMinBuildingDimensionsX(float NewScaleX, ETextCommit::Type TextCommitType) { mMinBuildingDimensions.X = NewScaleX; UpdateBuildingParams(mMinBuildingDimensions, mMaxBuildingDimensions); }
	void SetMinBuildingDimensionsY(float NewScaleY, ETextCommit::Type TextCommitType) { mMinBuildingDimensions.Y = NewScaleY; UpdateBuildingParams(mMinBuildingDimensions, mMaxBuildingDimensions); }
	void SetMinBuildingDimensionsZ(float NewScaleZ, ETextCommit::Type TextCommitType) { mMinBuildingDimensions.Z = NewScaleZ; UpdateBuildingParams(mMinBuildingDimensions, mMaxBuildingDimensions); }
	TOptional<float> GetMinBuildingDimensionsX() const { return mMinBuildingDimensions.X; }
	TOptional<float> GetMinBuildingDimensionsY() const { return mMinBuildingDimensions.Y; }
	TOptional<float> GetMinBuildingDimensionsZ() const { return mMinBuildingDimensions.Z; }
	/*--Max--*/
	void SetMaxBuildingDimensionsX(float NewScaleX, ETextCommit::Type TextCommitType) { mMaxBuildingDimensions.X = NewScaleX; UpdateBuildingParams(mMinBuildingDimensions, mMaxBuildingDimensions); }
	void SetMaxBuildingDimensionsY(float NewScaleY, ETextCommit::Type TextCommitType) { mMaxBuildingDimensions.Y = NewScaleY; UpdateBuildingParams(mMinBuildingDimensions, mMaxBuildingDimensions); }
	void SetMaxBuildingDimensionsZ(float NewScaleZ, ETextCommit::Type TextCommitType) { mMaxBuildingDimensions.Z = NewScaleZ; UpdateBuildingParams(mMinBuildingDimensions, mMaxBuildingDimensions); }
	TOptional<float> GetMaxBuildingDimensionsX() const { return mMaxBuildingDimensions.X; }
	TOptional<float> GetMaxBuildingDimensionsY() const { return mMaxBuildingDimensions.Y; }
	TOptional<float> GetMaxBuildingDimensionsZ() const { return mMaxBuildingDimensions.Z; }

public:
	PrototypeLevelGeneratorWindow();

	SLATE_BEGIN_ARGS(PrototypeLevelGeneratorWindow)
	{

	}
	SLATE_END_ARGS()

	void Construct(const FArguments& args);
};