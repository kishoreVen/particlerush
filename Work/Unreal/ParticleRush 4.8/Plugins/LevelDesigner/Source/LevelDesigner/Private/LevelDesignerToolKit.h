// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

/** Geometry Mode widget for controls */
class SLevelDesignerModeControls : public SCompoundWidget, public FNotifyHook
{
public:
	SLATE_BEGIN_ARGS(SLevelDesignerModeControls) {}
	SLATE_END_ARGS()

public:

	/** SCompoundWidget functions */
	void Construct(const FArguments& InArgs);

protected:

private:
	void CreateLayout();

	/** Returns a reference to the geometry mode tool */
	class FModeTool* GetModeTool() const;

private:
	/** Pointer to the parent window, so we know to destroy it when done */
	TWeakPtr<SWindow> ParentWindow;
};

/**
 * Mode Toolkit for the Geometry Tools
 */
class FLevelDesignerToolKit : public FModeToolkit
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	/** Initializes the geometry mode toolkit */
	virtual void Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override;

private:
	/** Geometry tools widget */
	TSharedPtr<class SLevelDesignerModeControls> LevelGeneratorWidget;
};
