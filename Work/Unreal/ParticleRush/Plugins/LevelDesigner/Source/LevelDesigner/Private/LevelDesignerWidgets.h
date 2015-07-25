// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LevelDesignerEdMode.h"

class FLevelDesigner_EraseModeTool;
class FLevelDesigner_DesignModeTool;

class SLevelDesignerEraseModeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelDesignerEraseModeWidget) {}
	SLATE_END_ARGS()

	SLevelDesignerEraseModeWidget();
	~SLevelDesignerEraseModeWidget();

	void Construct(const FArguments& InArgs, FLevelDesigner_EraseModeTool* InWidgetTool);

	/* Properties */
	TOptional<float> GetBrushRadius() const;
	void SetBrushRadius(float value);

private:
	TSharedPtr<FLevelDesigner_EraseModeTool> WidgetTool;
};


class SLevelDesignerDesignModeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelDesignerDesignModeWidget) {}
	SLATE_END_ARGS()

	SLevelDesignerDesignModeWidget();
	~SLevelDesignerDesignModeWidget();

	void Construct(const FArguments& InArgs, FLevelDesigner_DesignModeTool* InWidgetTool);

	/* Properties */
	TOptional<int32> GetNumBuildingClasses() const;
	void SetNumBuildingClasses(int32 value);

	TOptional<float> GetDefaultAlleySpacingX() const;
	void SetDefaultAlleySpacingX(float value);

	TOptional<float> GetDefaultAlleySpacingY() const;
	void SetDefaultAlleySpacingY(float value);

	TOptional<float> GetRotationalVariance() const;
	void SetRotationalVariance(float value);

private:
	TSharedPtr<FLevelDesigner_DesignModeTool> WidgetTool;

	int32 GetSelectedPanel() const;

	bool IsBuildingClassEnabled(int32 classId) const;

	FReply SetCurrentBuildingClass(int32 classId);

	int32 mCurrentlySelectedBuildingClass;
};