// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LevelDesignerPrivatePCH.h"

#include "UnrealEd.h"
#include "ObjectTools.h"

#include "ScopedTransaction.h"

#include "ModuleManager.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Toolkits/ToolkitManager.h"


//Slate dependencies
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Editor/LevelEditor/Public/SLevelViewport.h"

// Classes
#include "Landscape.h"
#include "LandscapeProxy.h"
#include "LandscapeComponent.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "LandscapeInfo.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/CollisionProfile.h"
#include "EngineUtils.h"
#include "LandscapeDataAccess.h"
#include "Components/BrushComponent.h"

#include "LevelDesignerEditCommands.h"
#include "LevelDesignerToolKit.h"

#include "LevelDesignerEdMode.h"
#include "LevelDesignerTools.h"
#include "LevelDesignerBuildingClass.h"

#define LOCTEXT_NAMESPACE "LevelDesignerEdMode"

DEFINE_LOG_CATEGORY(LogLevelDesignerEdMode)

//
// FLevelDesignerEdMode
//

const FEditorModeID FLevelDesignerEdMode::EM_LevelDesigner(TEXT("EM_LevelDesigner"));

/** Constructor */
FLevelDesignerEdMode::FLevelDesignerEdMode()
	: FEdMode()
{
	Tools.Add(new FLevelDesigner_EraseModeTool(UISettings));
	Tools.Add(new FLevelDesigner_DesignModeTool());

	SetCurrentLevelDesignerTool(ELevelDesignerModeTools::MT_Design);

	FLevelDesignerEditCommands::Register();
	UICommandList = MakeShareable(new FUICommandList);
	BindCommands();
}

void FLevelDesignerEdMode::SetCurrentLevelDesignerTool(ELevelDesignerModeTools ToolModeID)
{
	FModeTool* currentTool = GetCurrentTool();
	if (currentTool != NULL)
		currentTool->EndModify();

	SetCurrentTool((EModeTools)ToolModeID);

	FModeTool* newTool = FindTool((EModeTools)ToolModeID);
	if (newTool != NULL)
		newTool->StartModify();
}

bool FLevelDesignerEdMode::CurrentToolUsesBrush()
{
	return GetCurrentTool()->GetID() == (EModeTools)ELevelDesignerModeTools::MT_Erase;
}

void FLevelDesignerEdMode::BindCommands()
{
	const FLevelDesignerEditCommands& Commands = FLevelDesignerEditCommands::Get();

	UICommandList->MapAction(
		Commands.EraseMode,
		FExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::OnSetEraseTool),
		FCanExecuteAction(),
		FIsActionChecked::CreateLambda([=]
		{
			return GetCurrentTool()->GetID() == (EModeTools)ELevelDesignerModeTools::MT_Erase;
		})
		);

	UICommandList->MapAction(
		Commands.EraseMode,
		FExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::OnSetDesignTool),
		FCanExecuteAction(),
		FIsActionChecked::CreateLambda([=]
		{
			return GetCurrentTool()->GetID() == (EModeTools)ELevelDesignerModeTools::MT_Design;
		})
		);
}

/** Destructor */
FLevelDesignerEdMode::~FLevelDesignerEdMode()
{
	// Save UI settings to config file
	UISettings.Save();
	FEditorDelegates::MapChange.RemoveAll(this);
}


/** FGCObject interface */
void FLevelDesignerEdMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	// Call parent implementation
	FEdMode::AddReferencedObjects(Collector);
}

void FLevelDesignerEdMode::OnObjectsReplaced(const TMap<UObject*, UObject*>& ReplacementMap)
{
	UWorld* World = GetWorld();
	ULevel* CurrentLevel = World->GetCurrentLevel();
	const int32 NumLevels = World->GetNumLevels();

	// See if any IFA needs to update a foliage type reference
	for (int32 LevelIdx = 0; LevelIdx < NumLevels; ++LevelIdx)
	{
		ULevel* Level = World->GetLevel(LevelIdx);
		if (Level && Level->bIsVisible)
		{
			
		}
	}
}

/** FEdMode: Called when the mode is entered */
void FLevelDesignerEdMode::Enter()
{
	FEdMode::Enter();

	// register for any objects replaced
	GEditor->OnObjectsReplaced().AddRaw(this, &FLevelDesignerEdMode::OnObjectsReplaced);

	// Clear any selection in case the instanced foliage actor is selected
	GEditor->SelectNone(true, true);

	// Load UI settings from config file
	UISettings.Load();

	// Force real-time viewports.  We'll back up the current viewport state so we can restore it when the
	// user exits this mode.
	const bool bWantRealTime = true;
	const bool bRememberCurrentState = true;
	ForceRealTimeViewports(bWantRealTime, bRememberCurrentState);

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FLevelDesignerToolKit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
	
	// Subscribe to mesh changed events (for existing and future IFA's)
}

/** FEdMode: Called when the mode is exited */
void FLevelDesignerEdMode::Exit()
{
	FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
	Toolkit.Reset();

	GEditor->OnObjectsReplaced().RemoveAll(this);
	
	// Restore real-time viewport state if we changed it
	const bool bWantRealTime = false;
	const bool bRememberCurrentState = false;
	ForceRealTimeViewports(bWantRealTime, bRememberCurrentState);

	// Clear the cache (safety, should be empty!)
	LandscapeLayerCaches.Empty();

	// Save UI settings to config file
	UISettings.Save();

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FLevelDesignerEdMode::PostUndo()
{
	FEdMode::PostUndo();
}

/** When the user changes the current tool in the UI */
void FLevelDesignerEdMode::HandleToolChanged()
{
	OnToolChanged.Broadcast();
}

void FLevelDesignerEdMode::OnSetEraseTool()
{
	HandleToolChanged();
}

void FLevelDesignerEdMode::OnSetDesignTool()
{
	HandleToolChanged();
}

bool FLevelDesignerEdMode::DisallowMouseDeltaTracking() const
{
	bool isToolActive = false;/*GetCurrentTool()->GetID() == (EModeTools)ELevelDesignerModeTools::MT_Erase;*/

	if (isToolActive == true)
	{
		isToolActive = ((FLevelDesigner_BaseModeTool*)GetCurrentTool())->IsToolActive();
	}
	// We never want to use the mouse delta tracker while painting
	return isToolActive;
}

/** FEdMode: Called once per frame */
void FLevelDesignerEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);

	//for (const auto& Tool : Tools)
	//{
	//	Tool->Tick(ViewportClient, DeltaTime);
	//}
}

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
bool FLevelDesignerEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 MouseX, int32 MouseY)
{
	//for (const auto& Tool : Tools)
	//{
	//	Tool->MouseMove(ViewportClient, Viewport, MouseX, MouseY);
	//}

	return FEdMode::MouseMove(ViewportClient, Viewport, MouseX, MouseY);
}

/**
 * Called when the mouse is moved while a window input capture is in effect
 *
 * @param	InViewportClient	Level editor viewport client that captured the mouse input
 * @param	InViewport			Viewport that captured the mouse input
 * @param	InMouseX			New mouse cursor X coordinate
 * @param	InMouseY			New mouse cursor Y coordinate
 *
 * @return	true if input was handled
 */
bool FLevelDesignerEdMode::CapturedMouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 MouseX, int32 MouseY)
{
	return false;
}


/** FEdMode: Called when a key is pressed */
bool FLevelDesignerEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (Event != IE_Released)
	{
		if (UICommandList->ProcessCommandBindings(Key, FSlateApplication::Get().GetModifierKeys(), false/*Event == IE_Repeat*/))
		{
			return true;
		}
	}
	 
	bool bHandled = false;
	//for (const auto& Tool : Tools)
	//{
	//	bHandled = Tool->InputKey(ViewportClient, Viewport, Key, Event);
	//	if (bHandled == true)
	//		break;
	//}

	return bHandled;
}

/** FEdMode: Render the foliage edit mode */
void FLevelDesignerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	/** Call parent implementation */
	FEdMode::Render(View, Viewport, PDI);

	//for (const auto& Tool : Tools)
	//{
	//	Tool->Render(View, Viewport, PDI);
	//}
}


/** FEdMode: Render HUD elements for this tool */
void FLevelDesignerEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	//for (const auto& Tool : Tools)
	//{
	//	Tool->DrawHUD(ViewportClient, Viewport, View, Canvas);
	//}
}

/** FEdMode: Check to see if an actor can be selected in this mode - no side effects */
bool FLevelDesignerEdMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	return ((FLevelDesigner_BaseModeTool*)GetCurrentTool())->IsSelectionAllowed();
}

/** FEdMode: Called when the currently selected actor has changed */
void FLevelDesignerEdMode::ActorSelectionChangeNotify()
{
}

/** Forces real-time perspective viewports */
void FLevelDesignerEdMode::ForceRealTimeViewports(const bool bEnable, const bool bStoreCurrentState)
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr< ILevelViewport > ViewportWindow = LevelEditorModule.GetFirstActiveViewport();
	if (ViewportWindow.IsValid())
	{
		FEditorViewportClient &Viewport = ViewportWindow->GetLevelViewportClient();
		if (Viewport.IsPerspective())
		{
			if (bEnable)
			{
				Viewport.SetRealtime(bEnable, bStoreCurrentState);
			}
			else
			{
				const bool bAllowDisable = true;
				Viewport.RestoreRealtime(bAllowDisable);
			}
		}
	}
}

bool FLevelDesignerEdMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy *HitProxy, const FViewportClick &Click)
{
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}

FVector FLevelDesignerEdMode::GetWidgetLocation() const
{
	return FEdMode::GetWidgetLocation();
}

/** FEdMode: Called when a mouse button is pressed */
bool FLevelDesignerEdMode::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	UE_LOG(LogLevelDesignerEdMode, Warning, TEXT("Start Tracking Editor"));
	
	return FEdMode::StartTracking(InViewportClient, InViewport);
}

/** FEdMode: Called when the a mouse button is released */
bool FLevelDesignerEdMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	return FEdMode::EndTracking(InViewportClient, InViewport);
}

/** FEdMode: Called when mouse drag input it applied */
bool FLevelDesignerEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	//for (const auto& Tool : Tools)
	//{
	//	Tool->InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
	//}

	return FEdMode::InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
}

bool FLevelDesignerEdMode::AllowWidgetMove()
{
	return ShouldDrawWidget();
}

bool FLevelDesignerEdMode::UsesTransformWidget() const
{
	return ShouldDrawWidget();
}

bool FLevelDesignerEdMode::ShouldDrawWidget() const
{
	return true;
}

bool FLevelDesignerEdMode::ShowModeWidgets() const
{
	return true;
}

bool FLevelDesignerEdMode::UsesToolkits() const
{
	return true;
}


EAxisList::Type FLevelDesignerEdMode::GetWidgetAxisToDraw(FWidget::EWidgetMode InWidgetMode) const
{
	switch (InWidgetMode)
	{
	case FWidget::WM_Translate:
	case FWidget::WM_Rotate:
	case FWidget::WM_Scale:
		return EAxisList::XYZ;
	default:
		return EAxisList::None;
	}
}

/** Load UI settings from ini file */
void FLevelDesignerUISettings::Load()
{
	GConfig->GetFloat(	TEXT("LevelDesigner"),	TEXT("BrushRadius"),				BrushRadius,						GEditorPerProjectIni);
}

/** Save UI settings to ini file */
void FLevelDesignerUISettings::Save()
{
	GConfig->SetFloat(	TEXT("LevelDesigner"),	TEXT("BrushRadius"),				BrushRadius,						GEditorPerProjectIni);
}

#undef LOCTEXT_NAMESPACE