// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once


// Forward declarations
class ULandscapeComponent;
class FLevelDesignerBuilding;

// Log Declarations
DECLARE_LOG_CATEGORY_EXTERN(LogLevelDesignerEdMode, Log, All)

// Current user settings in Foliage UI
struct FLevelDesignerUISettings
{
	void Load();
	void Save();

	// tool
	float GetBrushRadius() const { return BrushRadius; }
	void SetBrushRadius(float value) { BrushRadius = value; }

	FLevelDesignerUISettings()
		: BrushRadius(512.f)
	{
	}

	~FLevelDesignerUISettings()
	{
	}

	
private:
	/* Brush Settings */
	float BrushRadius;
};

/**
 * Foliage editor mode
 */
enum ELevelDesignerModeTools : int8
{
	MT_Erase	= 0,
	MT_Design	= 1
};


class FLevelDesignerEdMode : public FEdMode
{
public:
	static const FEditorModeID EM_LevelDesigner;

	FLevelDesignerUISettings UISettings;

	/** Command list lives here so that the key bindings on the commands can be processed in the viewport. */
	TSharedPtr<FUICommandList> UICommandList;

	/** Constructor */
	FLevelDesignerEdMode();

	/** Destructor */
	virtual ~FLevelDesignerEdMode();

	/* Set Tool wrapper */
	void SetCurrentLevelDesignerTool(ELevelDesignerModeTools ToolModeID);

	/** FGCObject interface */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	void OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap);

	/** FEdMode: Called when the mode is entered */
	virtual void Enter() override;

	/** FEdMode: Called when the mode is exited */
	virtual void Exit() override;

	/** FEdMode: Called after an Undo operation */
	virtual void PostUndo() override;

	virtual bool UsesToolkits() const override;
	
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

	virtual bool ShowModeWidgets() const override;

	virtual bool DisallowMouseDeltaTracking() const override;

	/** Forces real-time perspective viewports */
	void ForceRealTimeViewports(const bool bEnable, const bool bStoreCurrentState);

	typedef TMap<FName, TMap<ULandscapeComponent*, TArray<uint8> > > LandscapeLayerCacheData;

	FSimpleMulticastDelegate OnToolChanged;
private:

	void BindCommands();

	/** Called when the user changes the current tool in the UI */
	void HandleToolChanged();

	/** Sets the tool mode to Erase. */
	void OnSetEraseTool();

	/** Sets the tool mode to Design. */
	void OnSetDesignTool();

	bool CurrentToolUsesBrush();

	// Landscape layer cache data
	LandscapeLayerCacheData LandscapeLayerCaches;
};

