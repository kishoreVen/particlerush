// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "EditorModes.h"
#include "EditorModeTools.h"

#include "LevelDesignerEdMode.h"

/*
* Base Mode Tool class for LevelDesignerTools
*/
class FLevelDesigner_BaseModeTool : public FModeTool
{
public:
	FLevelDesigner_BaseModeTool() {}
	~FLevelDesigner_BaseModeTool() {}

	virtual TSharedRef<SVerticalBox> MakeWidget() = 0;

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) { return false; };
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) { return true; };

	virtual bool IsToolActive() { return false; }

	virtual bool IsSelectionAllowed() { return true; }
};


/**
 * Widget manipulation of geometry.
 */
class FLevelDesigner_EraseModeTool : public FLevelDesigner_BaseModeTool
{
public:
	FLevelDesigner_EraseModeTool(FLevelDesignerUISettings& InUISettings);
	~FLevelDesigner_EraseModeTool();

	virtual FString GetName() const override { return TEXT("EraseModeTool"); }

	/**
	 * @return		true if the delta was handled by this editor mode tool.
	 */
	virtual bool InputDelta(FEditorViewportClient* InViewportClient,FViewport* InViewport,FVector& InDrag,FRotator& InRot,FVector& InScale) override;


	virtual bool StartModify() override;
	virtual bool EndModify() override;

	virtual void StartTrans() override;
	virtual void EndTrans() override;

	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	virtual void SelectNone() override;
	virtual bool BoxSelect( FBox& InBox, bool InSelect = true ) override;
	virtual bool FrustumSelect( const FConvexVolume& InFrustum, bool InSelect = true ) override;

	virtual void Tick(FEditorViewportClient* ViewportClient,float DeltaTime) override;

	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;

	/** @return		true if the key was handled by this editor mode tool. */
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;

	virtual void Render(const FSceneView* View,FViewport* Viewport,FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient,FViewport* Viewport,const FSceneView* View,FCanvas* Canvas) override;

	virtual TSharedRef<SVerticalBox> MakeWidget() override;

	/* Properties*/
	float GetBrushRadius() const;
	void SetBrushRadius(float value);

	virtual bool IsToolActive() { return bIsToolActive; }

	virtual bool IsSelectionAllowed() override { return false; }
	
protected:	

private:
	void BrushTrace(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY);

	bool bBrushTraceValid;
	bool bIsToolActive;
	bool bAdjustBrushRadius;
	bool bIsDragging;

	float BrushRadius;

	FVector BrushLocation;
	FVector BrushTraceDirection;
	UStaticMeshComponent* SphereBrushComponent;

	FLevelDesignerUISettings& UISettings;

	void AdjustBrushRadius(float adjustmentAmount);
};


class FLevelDesigner_DesignModeTool : public FLevelDesigner_BaseModeTool
{
public:
	FLevelDesigner_DesignModeTool(FLevelDesignerUISettings& InUISettings);

	virtual FString GetName() const override { return TEXT("DesignModeTool"); }

	/**
	 * @return		true if the delta was handled by this editor mode tool.
	 */
	virtual bool InputDelta(FEditorViewportClient* InViewportClient,FViewport* InViewport,FVector& InDrag,FRotator& InRot,FVector& InScale) override;

	virtual bool StartModify() override;
	virtual bool EndModify() override;

	virtual void StartTrans() override;
	virtual void EndTrans() override;

	virtual void SelectNone() override;
	virtual bool BoxSelect( FBox& InBox, bool InSelect = true ) override;
	virtual bool FrustumSelect( const FConvexVolume& InFrustum, bool InSelect = true ) override;

	virtual void Tick(FEditorViewportClient* ViewportClient,float DeltaTime) override;

	/** @return		true if the key was handled by this editor mode tool. */
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;

	virtual void Render(const FSceneView* View,FViewport* Viewport,FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient,FViewport* Viewport,const FSceneView* View,FCanvas* Canvas) override;

	virtual TSharedRef<SVerticalBox> MakeWidget() override;

	/* Properties */
	void SetNumBuildingClasses(int32 value);
	int32 GetNumBuildingClasses() const;

	FVector2D GetDefaultAlleySpacing() const;
	void SetDefaultAlleySpacing(const FVector2D& value);

	void SetRotationalVariance(float value);
	float GetRotationalVariance() const;

protected:

private:
	FLevelDesignerUISettings& UISettings;
};

