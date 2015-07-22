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

#define LOCTEXT_NAMESPACE "LevelDesignerEdMode"

DEFINE_LOG_CATEGORY(LogLevelDesignerEdMode)

//
// FLevelDesignerEdMode
//

const FEditorModeID FLevelDesignerEdMode::EM_LevelDesigner(TEXT("EM_LevelDesigner"));

/** Constructor */
FLevelDesignerEdMode::FLevelDesignerEdMode()
	: FEdMode()
	, bToolActive(false)
	, bIsDragging(false)
	, bAdjustBrushRadius(false)
{
	// Load resources and construct brush component
	UMaterial* BrushMaterial = nullptr;
	UStaticMesh* StaticMesh = nullptr;
	if (!IsRunningCommandlet())
	{
		BrushMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/EditorLandscapeResources/FoliageBrushSphereMaterial.FoliageBrushSphereMaterial"), nullptr, LOAD_None, nullptr);
		StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineMeshes/Sphere.Sphere"), nullptr, LOAD_None, nullptr);
	}

	SphereBrushComponent = NewObject<UStaticMeshComponent>(GetTransientPackage(), TEXT("SphereBrushComponent"));
	SphereBrushComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SphereBrushComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereBrushComponent->StaticMesh = StaticMesh;
	SphereBrushComponent->OverrideMaterials.Add(BrushMaterial);
	SphereBrushComponent->SetAbsolute(true, true, true);
	SphereBrushComponent->CastShadow = false;

	bBrushTraceValid = false;
	BrushLocation = FVector::ZeroVector;

	FLevelDesignerEditCommands::Register();
	UICommandList = MakeShareable(new FUICommandList);
	BindCommands();
}

void FLevelDesignerEdMode::BindCommands()
{
	const FLevelDesignerEditCommands& Commands = FLevelDesignerEditCommands::Get();

	UICommandList->MapAction(
		Commands.IncreaseBrushSize,
		FExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::AdjustBrushRadius, 50.f),
		FCanExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::CurrentToolUsesBrush));

	UICommandList->MapAction(
		Commands.DecreaseBrushSize,
		FExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::AdjustBrushRadius, -50.f),
		FCanExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::CurrentToolUsesBrush));

	UICommandList->MapAction(
		Commands.SetErase,
		FExecuteAction::CreateRaw(this, &FLevelDesignerEdMode::OnSetEraseTool),
		FCanExecuteAction(),
		FIsActionChecked::CreateLambda([=]
		{
			return UISettings.bEraseToolSelected;
		}));
}

bool FLevelDesignerEdMode::CurrentToolUsesBrush() const
{
	return UISettings.bEraseToolSelected;
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

	Collector.AddReferencedObject(SphereBrushComponent);
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
	
	// Remove the brush
	SphereBrushComponent->UnregisterComponent();

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

void FLevelDesignerEdMode::ClearAllToolSelection()
{
	UISettings.bEraseToolSelected = false;
}

void FLevelDesignerEdMode::OnSetEraseTool()
{
	ClearAllToolSelection();
	UISettings.bEraseToolSelected = true;
	HandleToolChanged();
}

bool FLevelDesignerEdMode::DisallowMouseDeltaTracking() const
{
	// We never want to use the mouse delta tracker while painting
	return bToolActive;
}

/** FEdMode: Called once per frame */
void FLevelDesignerEdMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	if (bToolActive)
	{
		ApplyBrush(ViewportClient);
	}

	FEdMode::Tick(ViewportClient, DeltaTime);

	// Update the position and size of the brush component
	if (bBrushTraceValid && UISettings.IsAnyToolActive())
	{
		// Scale adjustment is due to default sphere SM size.
		FTransform BrushTransform = FTransform(FQuat::Identity, BrushLocation, FVector(UISettings.GetRadius() * 0.00625f));
		SphereBrushComponent->SetRelativeTransform(BrushTransform);

		if (!SphereBrushComponent->IsRegistered())
		{
			SphereBrushComponent->RegisterComponentWithWorld(ViewportClient->GetWorld());
		}
	}
	else
	{
		if (SphereBrushComponent->IsRegistered())
		{
			SphereBrushComponent->UnregisterComponent();
		}
	}
}

void FLevelDesignerEdMode::PreApplyBrush()
{

}

void FLevelDesignerEdMode::ApplyBrush(FEditorViewportClient* ViewportClient)
{
	if (!bBrushTraceValid || ViewportClient != GCurrentLevelEditingViewportClient)
	{
		return;
	}

	float BrushArea = PI * FMath::Square(UISettings.GetRadius());

	// Tablet pressure
	float Pressure = ViewportClient->Viewport->IsPenActive() ? ViewportClient->Viewport->GetTabletPressure() : 1.f;

	// Cache a copy of the world pointer
	UWorld* World = ViewportClient->GetWorld();
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
	BrushTrace(ViewportClient, MouseX, MouseY);

	return false;
}

void FLevelDesignerEdMode::BrushTrace(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY)
{
	bBrushTraceValid = false;
	bool bIsMovingCamera = ViewportClient->IsMovingCamera();
	bool bIsViewPortvisible = ViewportClient->IsVisible();
	
	if (!bIsMovingCamera && bIsViewPortvisible)
	{
		if (UISettings.IsAnyToolActive())
		{
			// Compute a world space ray from the screen space mouse coordinates
			FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
				ViewportClient->Viewport,
				ViewportClient->GetScene(),
				ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(ViewportClient->IsRealtime()));
			FSceneView* View = ViewportClient->CalcSceneView(&ViewFamily);
			FViewportCursorLocation MouseViewportRay(View, ViewportClient, MouseX, MouseY);

			FVector Start = MouseViewportRay.GetOrigin();
			BrushTraceDirection = MouseViewportRay.GetDirection();
			FVector End = Start + WORLD_MAX * BrushTraceDirection;

			FHitResult Hit;
			UWorld* World = ViewportClient->GetWorld();
			static FName NAME_FoliageBrush = FName(TEXT("FoliageBrush"));

			FCollisionQueryParams QueryParams(NAME_FoliageBrush, true);
			QueryParams.bReturnFaceIndex = false;

			float TraceRadius = 0.0f;

			//It's possible that with the radius of the shape we will end up with an initial overlap which would place the instance at the top of the procedural volume.
			//Moving the start trace back a bit will fix this, but it introduces the potential for spawning instances a bit above the volume. This second issue is already somewhat broken because of how sweeps work so it's not too bad, also this is a less common case.
			//The proper fix would be to do something like EncroachmentCheck where we first do a sweep, then we fix it up if it's overlapping, then check the filters. This is more expensive and error prone so for now we just move the trace up a bit.
			const FVector Dir = (End - Start).GetSafeNormal();
			const FVector StartTrace = Start - (Dir * TraceRadius);

			FCollisionShape SphereShape;
			SphereShape.SetSphere(TraceRadius);
			TArray<FHitResult> Hits;
			World->SweepMultiByObjectType(Hits, StartTrace, End, FQuat::Identity, FCollisionObjectQueryParams(ECC_WorldStatic), SphereShape, QueryParams);

			bool hitSuccessful = false;
			for (const FHitResult& HitItr : Hits)
			{
				ALandscape* collidedLandscape = Cast<ALandscape>(HitItr.Actor.Get());

				if (collidedLandscape != NULL)
				{
					Hit = HitItr;
					hitSuccessful = true;
					break;
				}
			}

			if (hitSuccessful)
			{
				// Check filters
				UPrimitiveComponent* PrimComp = Hit.Component.Get();
				UMaterialInterface* Material = PrimComp ? PrimComp->GetMaterial(0) : nullptr;

				if (PrimComp)
				{
					if (!bAdjustBrushRadius)
					{
						// Adjust the brush location
						BrushLocation = Hit.Location;
					}

					// Still want to draw the brush when resizing
					bBrushTraceValid = true;
				}
			}
		}
	}
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
	BrushTrace(ViewportClient, MouseX, MouseY);
	return false;
}

void FLevelDesignerEdMode::GetRandomVectorInBrush(FVector& OutStart, FVector& OutEnd)
{
	// Find Rx and Ry inside the unit circle
	float Ru = (2.f * FMath::FRand() - 1.f);
	float Rv = (2.f * FMath::FRand() - 1.f) * FMath::Sqrt(1.f - FMath::Square(Ru));

	// find random point in circle thru brush location parallel to screen surface
	FVector U, V;
	BrushTraceDirection.FindBestAxisVectors(U, V);
	FVector Point = Ru * U + Rv * V;

	// find distance to surface of sphere brush from this point
	FVector Rw = FMath::Sqrt(1.f - (FMath::Square(Ru) + FMath::Square(Rv))) * BrushTraceDirection;

	OutStart	= BrushLocation + UISettings.GetRadius() * (Point - Rw);
	OutEnd		= BrushLocation + UISettings.GetRadius() * (Point + Rw);
}

void FLevelDesignerEdMode::AdjustBrushRadius(float Adjustment)
{
	const float CurrentBrushRadius = UISettings.GetRadius();

	if (Adjustment > 0.f)
	{
		UISettings.SetRadius(FMath::Min(CurrentBrushRadius + Adjustment, 8192.f));
	}
	else if (Adjustment < 0.f)
	{
		UISettings.SetRadius(FMath::Max(CurrentBrushRadius + Adjustment, 0.f));
	}
	
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
	if (UISettings.IsAnyToolActive())
	{
		// Require Ctrl or not as per user preference
		
		if (Key == EKeys::LeftMouseButton && Event == IE_Pressed)
		{
			// Only activate tool if we're not already moving the camera and we're not trying to drag a transform widget
			// Not using "if (!ViewportClient->IsMovingCamera())" because it's wrong in ortho viewports :D
			bool bMovingCamera = Viewport->KeyState(EKeys::MiddleMouseButton) || Viewport->KeyState(EKeys::RightMouseButton) || IsAltDown(Viewport);

			if ((Viewport->IsPenActive() && Viewport->GetTabletPressure() > 0.f) ||
				(!bMovingCamera && ViewportClient->GetCurrentWidgetAxis() == EAxisList::None))
			{
				if (!bToolActive)
				{
					GEditor->BeginTransaction(NSLOCTEXT("UnrealEd", "PrototypeLevelGeneratorMode_EditTransaction", "Level Editing"));
					PreApplyBrush();
					ApplyBrush(ViewportClient);
					bToolActive = true;

					bHandled = true;
				}
			}
		}
		else if (bToolActive && Event == IE_Released &&
			(Key == EKeys::LeftMouseButton))
		{
			//Set the cursor position to that of the slate cursor so it wont snap back
			Viewport->SetPreCaptureMousePosFromSlateCursor();
			GEditor->EndTransaction();
			LandscapeLayerCaches.Empty();
			bToolActive = false;
			
			bHandled = true;
		}
		else if (IsCtrlDown(Viewport))
		{
			// Control + scroll adjusts the brush radius
			static const float RadiusAdjustmentAmount = 25.f;
			if (Key == EKeys::MouseScrollUp)
			{
				AdjustBrushRadius(RadiusAdjustmentAmount);
				
				bHandled = true;
			}
			else if (Key == EKeys::MouseScrollDown)
			{
				AdjustBrushRadius(-RadiusAdjustmentAmount);
				
				bHandled = true;
			}
		}
	}

	return bHandled;
}

/** FEdMode: Render the foliage edit mode */
void FLevelDesignerEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	/** Call parent implementation */
	FEdMode::Render(View, Viewport, PDI);
}


/** FEdMode: Render HUD elements for this tool */
void FLevelDesignerEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
}

/** FEdMode: Check to see if an actor can be selected in this mode - no side effects */
bool FLevelDesignerEdMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	return false;
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
	UE_LOG(LogLevelDesignerEdMode, Warning, TEXT("Clicked in Editor"));

	if (UISettings.IsAnyToolActive())
	{
		if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
		{
			GEditor->BeginTransaction(NSLOCTEXT("UnrealEd", "FPrototypeLevelGeneratorEdMode_EditTransaction", "Level Editing"));
			
			GEditor->EndTransaction();
		}

		return true;
	}


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

	if (IsCtrlDown(InViewport) && InViewport->KeyState(EKeys::MiddleMouseButton) && UISettings.IsAnyToolActive())
	{
		bAdjustBrushRadius = true;
		return true;
	}
	
	return FEdMode::StartTracking(InViewportClient, InViewport);
}

/** FEdMode: Called when the a mouse button is released */
bool FLevelDesignerEdMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	UE_LOG(LogLevelDesignerEdMode, Warning, TEXT("End Tracking Editor"));

	if (!bAdjustBrushRadius)
	{
		return true;
	}
	else
	{
		bAdjustBrushRadius = false;
		return true;
	}

	return FEdMode::EndTracking(InViewportClient, InViewport);
}

/** FEdMode: Called when mouse drag input it applied */
bool FLevelDesignerEdMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	if (bAdjustBrushRadius)
	{
		if (UISettings.IsAnyToolActive())
		{
			static const float RadiusAdjustmentFactor = 10.f;
			AdjustBrushRadius(RadiusAdjustmentFactor * InDrag.Y);
		}
	}

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
	GConfig->GetFloat(	TEXT("LevelDesigner"), TEXT("Radius"),						Radius,						GEditorPerProjectIni);
	GConfig->GetBool(	TEXT("LevelDesigner"), TEXT("bEraseToolSelected"),			bEraseToolSelected,			GEditorPerProjectIni);
}

/** Save UI settings to ini file */
void FLevelDesignerUISettings::Save()
{
	GConfig->SetFloat(	TEXT("LevelDesigner"),	TEXT("Radius"),						Radius,						GEditorPerProjectIni);
	GConfig->SetBool(	TEXT("LevelDesigner"),	TEXT("bEraseToolSelected"),			bEraseToolSelected,			GEditorPerProjectIni);
}

#undef LOCTEXT_NAMESPACE