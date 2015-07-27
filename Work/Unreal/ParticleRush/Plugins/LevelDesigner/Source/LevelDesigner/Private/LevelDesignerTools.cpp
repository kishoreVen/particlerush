// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LevelDesignerPrivatePCH.h"
#include "Engine/Selection.h"
#include "LevelDesignerTools.h"
#include "Landscape.h"
#include "LevelDesignerWidgets.h"
#include "LevelDesignerBuildingClass.h"
/*-----------------------------------------------------------------------------
	FLevelDesigner_EraseModeTool.
-----------------------------------------------------------------------------*/

#define LOCTEXT_NAMESPACE "LevelDesignerTools_Namespace"

FLevelDesigner_EraseModeTool::FLevelDesigner_EraseModeTool(FLevelDesignerUISettings& InUISettings)
:	bBrushTraceValid(false),
	bIsToolActive(false),
	bAdjustBrushRadius(false),
	bIsDragging(false),
	BrushLocation(FVector::ZeroVector),
	BrushTraceDirection(FVector::ZeroVector),
	UISettings(InUISettings)
{
	ID = (EModeTools)(ELevelDesignerModeTools::MT_Erase);

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
}

FLevelDesigner_EraseModeTool::~FLevelDesigner_EraseModeTool()
{
	SphereBrushComponent->UnregisterComponent();
}

void FLevelDesigner_EraseModeTool::SelectNone()
{
	
}

/** @return		true if something was selected/deselected, false otherwise. */
bool FLevelDesigner_EraseModeTool::BoxSelect( FBox& InBox, bool InSelect )
{
	bool bResult = false;
	
	return bResult;
}

/** @return		true if something was selected/deselected, false otherwise. */
bool FLevelDesigner_EraseModeTool::FrustumSelect( const FConvexVolume& InFrustum, bool InSelect /* = true */ )
{
	bool bResult = false;
	
	return bResult;
}

void FLevelDesigner_EraseModeTool::Tick(FEditorViewportClient* ViewportClient,float DeltaTime)
{
	FModeTool::Tick(ViewportClient, DeltaTime);

	// Update the position and size of the brush component
	if (bBrushTraceValid)
	{
		// Scale adjustment is due to default sphere SM size.
		FTransform BrushTransform = FTransform(FQuat::Identity, BrushLocation, FVector(GetBrushRadius() * 0.00625f));
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


	if (bIsDragging)
	{

	}
}

bool FLevelDesigner_EraseModeTool::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 MouseX, int32 MouseY)
{
	BrushTrace(ViewportClient, MouseX, MouseY);

	return false;
}

/**
 * @return		true if the delta was handled by this editor mode tool.
 */
bool FLevelDesigner_EraseModeTool::InputDelta(FEditorViewportClient* InViewportClient,FViewport* InViewport,FVector& InDrag,FRotator& InRot,FVector& InScale)
{
	if (bIsToolActive && bAdjustBrushRadius)
	{
		static const float RadiusAdjustmentFactor = 10.f;
		AdjustBrushRadius(RadiusAdjustmentFactor * InDrag.Y);
	}
	
	return false;
}

bool FLevelDesigner_EraseModeTool::StartModify()
{
	bIsToolActive = true;

	return true;
}

bool FLevelDesigner_EraseModeTool::EndModify()
{
	bIsToolActive = false;

	return true;
}

void FLevelDesigner_EraseModeTool::StartTrans()
{
	
}

void FLevelDesigner_EraseModeTool::EndTrans()
{
	
}

bool FLevelDesigner_EraseModeTool::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	if (IsCtrlDown(InViewport) && InViewport->KeyState(EKeys::MiddleMouseButton) && bIsToolActive)
	{
		bAdjustBrushRadius = true;
		return true;
	}

	return false;
}

bool FLevelDesigner_EraseModeTool::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	if (bAdjustBrushRadius)
	{
		bAdjustBrushRadius = false;
		return true;
	}
	
	return false;
}

/**
 * @return		true if the key was handled by this editor mode tool.
 */
bool FLevelDesigner_EraseModeTool::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;
	if (Key == EKeys::LeftMouseButton && Event == IE_Pressed)
	{
		// Only activate tool if we're not already moving the camera and we're not trying to drag a transform widget
		// Not using "if (!ViewportClient->IsMovingCamera())" because it's wrong in ortho viewports :D
		bool bMovingCamera = Viewport->KeyState(EKeys::MiddleMouseButton) || Viewport->KeyState(EKeys::RightMouseButton) || IsAltDown(Viewport);
	
		if ((Viewport->IsPenActive() && Viewport->GetTabletPressure() > 0.f) ||
		(!bMovingCamera && ViewportClient->GetCurrentWidgetAxis() == EAxisList::None))
		{
			GEditor->BeginTransaction(NSLOCTEXT("UnrealEd", "PrototypeLevelGeneratorMode_EditTransaction", "Level Editing"));

			bIsToolActive = true;
			bHandled = true;
		}
	}
	else if (bIsToolActive && Event == IE_Released && (Key == EKeys::LeftMouseButton))
	{
		//Set the cursor position to that of the slate cursor so it wont snap back
		Viewport->SetPreCaptureMousePosFromSlateCursor();
		GEditor->EndTransaction();

		bIsToolActive = false;
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

	return bHandled;
}

void FLevelDesigner_EraseModeTool::DrawHUD(FEditorViewportClient* ViewportClient,FViewport* Viewport,const FSceneView* View,FCanvas* Canvas)
{

}

void FLevelDesigner_EraseModeTool::Render(const FSceneView* View,FViewport* Viewport,FPrimitiveDrawInterface* PDI)
{
	
}

void FLevelDesigner_EraseModeTool::BrushTrace(FEditorViewportClient* ViewportClient, int32 MouseX, int32 MouseY)
{
	bBrushTraceValid = false;
	bool bIsMovingCamera = ViewportClient->IsMovingCamera();
	bool bIsViewPortvisible = ViewportClient->IsVisible();

	if (!bIsMovingCamera && bIsViewPortvisible)
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

void FLevelDesigner_EraseModeTool::AdjustBrushRadius(float adjustmentAmount)
{
	float CurrentBrushRadius = GetBrushRadius();

	UISettings.SetBrushRadius(FMath::Clamp(CurrentBrushRadius + adjustmentAmount, 5.0f, 8192.f));
}


TSharedRef<SVerticalBox> FLevelDesigner_EraseModeTool::MakeWidget()
{
	TSharedRef<SVerticalBox> box = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SLevelDesignerEraseModeWidget, this)
		];

	return box;
}

float FLevelDesigner_EraseModeTool::GetBrushRadius() const
{ 
	return UISettings.GetBrushRadius(); 
}

void FLevelDesigner_EraseModeTool::SetBrushRadius(float value)
{
	UISettings.SetBrushRadius(value);
}

/*-----------------------------------------------------------------------------
	FLevelDesigner_DesignModeTool.
-----------------------------------------------------------------------------*/

FLevelDesigner_DesignModeTool::FLevelDesigner_DesignModeTool(FLevelDesignerUISettings& InUISettings)
: UISettings(InUISettings)
{
	ID = (EModeTools)(ELevelDesignerModeTools::MT_Design);
}

void FLevelDesigner_DesignModeTool::SelectNone()
{
	
}

/** @return		true if something was selected/deselected, false otherwise. */
bool FLevelDesigner_DesignModeTool::BoxSelect( FBox& InBox, bool InSelect )
{
	bool bResult = false;
	
	return bResult;
}

/** @return		true if something was selected/deselected, false otherwise. */
bool FLevelDesigner_DesignModeTool::FrustumSelect( const FConvexVolume& InFrustum, bool InSelect /* = true */ )
{
	bool bResult = false;
	
	return bResult;
}

void FLevelDesigner_DesignModeTool::Tick(FEditorViewportClient* ViewportClient,float DeltaTime)
{
	int x = 10;
	x++;
}

/**
 * @return		true if the delta was handled by this editor mode tool.
 */
bool FLevelDesigner_DesignModeTool::InputDelta(FEditorViewportClient* InViewportClient,FViewport* InViewport,FVector& InDrag,FRotator& InRot,FVector& InScale)
{	
	return false;
}

bool FLevelDesigner_DesignModeTool::StartModify()
{
	UpdateDesignerBuildings();

	return true;
}

bool FLevelDesigner_DesignModeTool::EndModify()
{
	return true;
}

void FLevelDesigner_DesignModeTool::StartTrans()
{
	
}

void FLevelDesigner_DesignModeTool::EndTrans()
{
	
}

/**
 * @return		true if the key was handled by this editor mode tool.
 */
bool FLevelDesigner_DesignModeTool::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	return FModeTool::InputKey(ViewportClient, Viewport, Key, Event);
}

void FLevelDesigner_DesignModeTool::DrawHUD(FEditorViewportClient* ViewportClient,FViewport* Viewport,const FSceneView* View,FCanvas* Canvas)
{

}

void FLevelDesigner_DesignModeTool::Render(const FSceneView* View,FViewport* Viewport,FPrimitiveDrawInterface* PDI)
{
	
}

TSharedRef<SVerticalBox> FLevelDesigner_DesignModeTool::MakeWidget()
{
	TSharedRef<SVerticalBox> box = SNew(SVerticalBox)
	+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SLevelDesignerDesignModeWidget, this)
		];

	return box;
}


int32 FLevelDesigner_DesignModeTool::GetNumBuildingClasses() const
{
	return UISettings.GetNumBuildingClasses();
}

void FLevelDesigner_DesignModeTool::SetNumBuildingClasses(int32 value)
{
	UISettings.SetNumBuildingClasses(value);
}

FVector2D FLevelDesigner_DesignModeTool::GetDefaultAlleySpacing() const
{
	return UISettings.GetDefaultAlleySpacing();
}

void FLevelDesigner_DesignModeTool::SetDefaultAlleySpacing(const FVector2D& value)
{
	UISettings.SetDefaultAlleySpacing(value);
}

void FLevelDesigner_DesignModeTool::SetRotationalVariance(float value)
{
	UISettings.SetRotationalVariance(value);
}

float FLevelDesigner_DesignModeTool::GetRotationalVariance() const
{
	return UISettings.GetRotationalVariance();
}

void FLevelDesigner_DesignModeTool::UpdateDesignerBuildings()
{
	static FColorList colorList;
	colorList.CreateColorMap();

	int32 numBuildings = UISettings.GetNumBuildingClasses();

	DesignerBuildings.Empty();

	int32 colorIndex = 2;

	for (int buildingIndex = 0; buildingIndex < numBuildings; buildingIndex++)
	{
		DesignerBuildings.Add(new FLevelDesignerBuilding(colorList.GetFColorByIndex(colorIndex)));

		colorIndex += 10;
	}
}