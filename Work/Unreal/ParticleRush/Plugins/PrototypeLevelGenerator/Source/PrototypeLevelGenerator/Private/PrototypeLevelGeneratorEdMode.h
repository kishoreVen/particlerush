// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once


// Forward declarations
class ULandscapeComponent;

// Log Declarations
DECLARE_LOG_CATEGORY_EXTERN(LevelGeneratorEdModeLog, Log, All)

// Current user settings in Foliage UI
struct FLevelGeneratorUISettings
{
	void Load();
	void Save();

	// tool
	float GetRadius() const { return Radius; }
	void SetRadius(float InRadius) { Radius = InRadius; }

	float GetPaintDensity() const { return PaintDensity; }
	void SetPaintDensity(float InPaintDensity) { PaintDensity = InPaintDensity; }
	
	float GetUnpaintDensity() const { return UnpaintDensity; }
	void SetUnpaintDensity(float InUnpaintDensity) { UnpaintDensity = InUnpaintDensity; }
	

	FLevelGeneratorUISettings()
		: Radius(512.f)
		, PaintDensity(0.5f)
		, UnpaintDensity(0.f)
		, bEraseToolSelected(true)
		, bRandomSpawnToolSelected(false)
	{
	}

	~FLevelGeneratorUISettings()
	{
	}

private:
	
	/* Brush Settings */
	float Radius;
	float PaintDensity;
	float UnpaintDensity;

public:
	bool bEraseToolSelected;
	bool bRandomSpawnToolSelected;
};

/**
 * Foliage editor mode
 */
class FPrototypeLevelGeneratorEdMode : public FEdMode
{
public:
	static const FEditorModeID EM_PrototypeLevelGenerator;

	FLevelGeneratorUISettings UISettings;

	/** Command list lives here so that the key bindings on the commands can be processed in the viewport. */
	TSharedPtr<FUICommandList> UICommandList;

	/** Constructor */
	FPrototypeLevelGeneratorEdMode();

	/** Destructor */
	virtual ~FPrototypeLevelGeneratorEdMode();

	/** FGCObject interface */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	void OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap);

	/** FEdMode: Called when the mode is entered */
	virtual void Enter() override;

	/** FEdMode: Called when the mode is exited */
	virtual void Exit() override;

	/** FEdMode: Called after an Undo operation */
	virtual void PostUndo() override;

	/** Called when asset is removed */
	void NotifyAssetRemoved(const FAssetData& AssetInfo);

	void BrushTrace(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY);
	
	/**
	 * Called when the mouse is moved over the viewport
	 *
	 * @param	InViewportClient	Level editor viewport client that captured the mouse input
	 * @param	InViewport			Viewport that captured the mouse input
	 * @param	InMouseX			New mouse cursor X coordinate
	 * @param	InMouseY			New mouse cursor Y coordinate
	 *
	 * @return	true if input was handled
	 */
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;

	/**
	 * FEdMode: Called when the mouse is moved while a window input capture is in effect
	 *
	 * @param	InViewportClient	Level editor viewport client that captured the mouse input
	 * @param	InViewport			Viewport that captured the mouse input
	 * @param	InMouseX			New mouse cursor X coordinate
	 * @param	InMouseY			New mouse cursor Y coordinate
	 *
	 * @return	true if input was handled
	 */
	virtual bool CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;

	/** FEdMode: Called when a mouse button is pressed */
	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	/** FEdMode: Called when a mouse button is released */
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport) override;

	/** FEdMode: Called once per frame */
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;

	/** FEdMode: Called when a key is pressed */
	virtual bool InputKey(FEditorViewportClient* InViewportClient, FViewport* InViewport, FKey InKey, EInputEvent InEvent) override;

	/** FEdMode: Called when mouse drag input it applied */
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;

	/** FEdMode: Render elements for the Foliage tool */
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;

	/** FEdMode: Render HUD elements for this tool */
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;

	/** FEdMode: Check to see if an actor can be selected in this mode - no side effects */
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;

	/** FEdMode: Called when the currently selected actor has changed */
	virtual void ActorSelectionChangeNotify() override;

	/** Notifies all active modes of mouse click messages. */
	bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click) override;

	/** FEdMode: widget handling */
	virtual FVector GetWidgetLocation() const override;
	virtual bool AllowWidgetMove() override;
	virtual bool ShouldDrawWidget() const override;
	virtual bool UsesTransformWidget() const override;
	virtual EAxisList::Type GetWidgetAxisToDraw(FWidget::EWidgetMode InWidgetMode) const override;

	virtual bool DisallowMouseDeltaTracking() const override;

	/** Forces real-time perspective viewports */
	void ForceRealTimeViewports(const bool bEnable, const bool bStoreCurrentState);

	/** Generate start/end points for a random trace inside the sphere brush.
	    returns a line segment inside the sphere parallel to the view direction */
	void GetRandomVectorInBrush(FVector& OutStart, FVector& OutEnd);

	/** Setup before call to ApplyBrush */
	void PreApplyBrush();

	/** Apply brush */
	void ApplyBrush(FEditorViewportClient* ViewportClient);

	/** Adjusts the radius of the foliage brush by the specified amount */
	void AdjustBrushRadius(float Adjustment);

	typedef TMap<FName, TMap<ULandscapeComponent*, TArray<uint8> > > LandscapeLayerCacheData;

	FSimpleMulticastDelegate OnToolChanged;
private:

	void BindCommands();
	bool CurrentToolUsesBrush() const;

	/** Called when the user changes the current tool in the UI */
	void HandleToolChanged();

	/** Deselects all tools */
	void ClearAllToolSelection();

	/** Sets the tool mode to Paint. */
	void OnSetEraseTool();

	bool bBrushTraceValid;
	FVector BrushLocation;
	FVector BrushTraceDirection;
	UStaticMeshComponent* SphereBrushComponent;

	// Landscape layer cache data
	LandscapeLayerCacheData LandscapeLayerCaches;

	bool bToolActive;
	bool bIsDragging;
	bool bAdjustBrushRadius;
};

