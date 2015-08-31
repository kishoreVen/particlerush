// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerWidgets.h"
#include "LevelDesignerTools.h"
#include "LevelDesignerStyle.h"
#include "LevelDesignerBuildingClass.h"
#include "GenerationBoxActor.h"

#define LOCTEXT_NAMESPACE "LevelDesignerWidgets_Namespace"

/* ----------------------------
Erase Mode Widget
-------------------------------*/

SLevelDesignerEraseModeWidget::SLevelDesignerEraseModeWidget()
{
	
}

SLevelDesignerEraseModeWidget::~SLevelDesignerEraseModeWidget()
{
	
}

TOptional<float> SLevelDesignerEraseModeWidget::GetBrushRadius() const 
{ 
	return WidgetTool->GetBrushRadius();
}

void SLevelDesignerEraseModeWidget::SetBrushRadius(float value) 
{ 
	WidgetTool->SetBrushRadius(value); 
}

void SLevelDesignerEraseModeWidget::Construct(const FArguments& InArgs, FLevelDesigner_EraseModeTool* InWidgetTool)
{
	WidgetTool = TSharedPtr<FLevelDesigner_EraseModeTool>(InWidgetTool);

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(STextBlock).Text(LOCTEXT("BrushRadius", "Brush Radius"))
		]

		+ SHorizontalBox::Slot()
			[
				SNew(SNumericEntryBox<float>)
				.AllowSpin(true)
				.MinValue(1.0f)
				.MaxValue(10000.0f)
				.MinSliderValue(1.0f)
				.MaxSliderValue(10000.0f)
				.Value(this, &SLevelDesignerEraseModeWidget::GetBrushRadius)
				.OnValueChanged(this, &SLevelDesignerEraseModeWidget::SetBrushRadius)
			]
	];
}


/* ----------------------------
Design Mode Widget
-------------------------------*/

SLevelDesignerDesignModeWidget::SLevelDesignerDesignModeWidget()
{
	
}

SLevelDesignerDesignModeWidget::~SLevelDesignerDesignModeWidget()
{
	
}

void SLevelDesignerDesignModeWidget::Construct(const FArguments& InArgs, FLevelDesigner_DesignModeTool* InWidgetTool)
{
	static FMargin ButtonMargin(3, 0, 3, 0);
	static FMargin DetailsMargin(0, 3, 0, 3);

	OnGenerationBoxAdded = InArgs._OnGenerationBoxAdded;
	WidgetTool = TSharedPtr<FLevelDesigner_DesignModeTool>(InWidgetTool);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SScrollBox)
			.Orientation(EOrientation::Orient_Horizontal)
			
			+ SScrollBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(ButtonMargin)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					.VAlign(EVerticalAlignment::VAlign_Center)
					.HAlign(EHorizontalAlignment::HAlign_Center)
					[
						SNew(SButton)
						.OnClicked(this, &SLevelDesignerDesignModeWidget::AddGenerationBox)
						.ToolTipText(LOCTEXT("AddTriggerBox", "Add a Trigger Box to generate objects within the volume"))
						[
							SNew(SImage)
							.Image(FLevelDesignerStyle::GetBrush(TEXT("LevelDesigner.BoxTrigger")))
						]
					]
					+ SVerticalBox::Slot()
						.AutoHeight()
						.VAlign(EVerticalAlignment::VAlign_Center)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("AddGenerationBox", "Add Generation Box"))
						]
				]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSeparator)
						.Orientation(Orient_Vertical)
					]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(ButtonMargin)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.VAlign(EVerticalAlignment::VAlign_Center)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						[
							SNew(SButton)
							.OnClicked(this, &SLevelDesignerDesignModeWidget::EditLevelDesignerSettings)
							.ToolTipText(LOCTEXT("EditSettings", "Edit Level Designer Settings"))
							[
								SNew(SImage)
								.Image(FLevelDesignerStyle::GetBrush(TEXT("LevelDesigner.EditSettings")))
							]
						]
						+ SVerticalBox::Slot()
							.AutoHeight()
							.VAlign(EVerticalAlignment::VAlign_Center)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("EditLevelSettings", "Edit Designer Settings"))
							]
					]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSeparator)
						.Orientation(Orient_Vertical)
					]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(ButtonMargin)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.VAlign(EVerticalAlignment::VAlign_Center)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						[
							SNew(SButton)
							.OnClicked(this, &SLevelDesignerDesignModeWidget::ClearLevel)
							.ToolTipText(LOCTEXT("DeleteAll", "Delete all generated objects and start a fresh"))
							[
								SNew(SImage)
								.Image(FLevelDesignerStyle::GetBrush(TEXT("LevelDesigner.DesignDelete")))
							]
						]
						+ SVerticalBox::Slot()
							.AutoHeight()
							.VAlign(EVerticalAlignment::VAlign_Center)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("ClearEntireLevel", "Clear Entire Level"))
							]
					]

				+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SSeparator)
						.Orientation(Orient_Vertical)
					]
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(3.0f)
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		]
	];
}

FReply SLevelDesignerDesignModeWidget::EditLevelDesignerSettings()
{
	FLevelDesignerModule& LevelDesignerModule = FModuleManager::LoadModuleChecked<FLevelDesignerModule>("LevelDesigner");
	UObject* LevelDesigenerSettingsAsset = LevelDesignerModule.GetLevelDesignerSettingsAsset();
	if (LevelDesigenerSettingsAsset != NULL)
		FAssetEditorManager::Get().OpenEditorForAsset(LevelDesigenerSettingsAsset);

	return FReply::Handled();
}

FReply SLevelDesignerDesignModeWidget::ClearLevel()
{
	return FReply::Handled();
}

UWorld* GetEditorWorld()
{
	UWorld* world = NULL;
	const TIndirectArray<FWorldContext>& contexts = GEngine->GetWorldContexts();

	for (int32 i = 0; i < contexts.Num(); i++)
	{
		if (contexts[i].WorldType == EWorldType::PIE)
			return contexts[i].World();

		if (contexts[i].WorldType == EWorldType::Editor)
			world = contexts[i].World();
	}

	return world;
}

FReply SLevelDesignerDesignModeWidget::AddGenerationBox()
{
	UWorld* World = GetEditorWorld();

	if (World == NULL)
		return FReply::Handled();

	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	AGenerationBoxActor* spawnedActor = World->SpawnActor<AGenerationBoxActor>(AGenerationBoxActor::StaticClass(), FVector(0, 0, 50), FRotator::ZeroRotator, spawnParams);
	spawnedActor->SetActorScale3D(FVector(100, 100, 100));

	OnGenerationBoxAdded.ExecuteIfBound(spawnedActor);

	return FReply::Handled();
}