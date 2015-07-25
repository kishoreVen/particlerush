// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerWidgets.h"
#include "LevelDesignerTools.h"
#include "LevelDesignerStyle.h"

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
: mCurrentlySelectedBuildingClass(0)
{
	
}

SLevelDesignerDesignModeWidget::~SLevelDesignerDesignModeWidget()
{
	
}

void SLevelDesignerDesignModeWidget::Construct(const FArguments& InArgs, FLevelDesigner_DesignModeTool* InWidgetTool)
{
	static FMargin ButtonMargin(3, 0, 3, 0);
	static FMargin DetailsMargin(0, 3, 0, 3);

	WidgetTool = TSharedPtr<FLevelDesigner_DesignModeTool>(InWidgetTool);

	TSharedRef<SWidgetSwitcher> WidgetSwitcher = SNew(SWidgetSwitcher)
												.WidgetIndex(this, &SLevelDesignerDesignModeWidget::GetSelectedPanel);

	TSharedRef<SScrollBox> SelectorBox = SNew(SScrollBox)
		.Orientation(EOrientation::Orient_Horizontal);

	for (int x = 0; x < 4; x++)
	{
		WidgetSwitcher->AddSlot()
			[
				SNullWidget::NullWidget
			];

		SelectorBox->AddSlot()
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "ToggleButton")
				.OnClicked(this, &SLevelDesignerDesignModeWidget::SetCurrentBuildingClass, x)
				.IsEnabled(this, &SLevelDesignerDesignModeWidget::IsBuildingClassEnabled, x)
				[
					SNew(SImage)
				]
			];
	}

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(ButtonMargin)
			[
				SNew(SButton)
				[
					SNew(SImage)
					.Image(FLevelDesignerStyle::GetBrush(TEXT("LevelDesigner.BoxTrigger")))
					.ToolTipText(LOCTEXT("AddTriggerBox", "Add a Trigger Box to generate objects within the volume"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(ButtonMargin)
			[
				SNew(SButton)
				[
					SNew(SImage)
					.Image(FLevelDesignerStyle::GetBrush(TEXT("LevelDesigner.DesignDelete")))
					.ToolTipText(LOCTEXT("DeleteAll", "Delete all generated objects and start a fresh"))
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


		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSplitter)
			.Orientation(EOrientation::Orient_Horizontal)

			+SSplitter::Slot()
			.Value(0.55f)
			[ 
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.MaxHeight(50.0f)
				.Padding(DetailsMargin)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("NumBuildingClasses", "Number of Building Classes"))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50.0f)
				.Padding(DetailsMargin)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("DefaultSpacing", "Default Spacing Between Buildings"))
				]
				+ SVerticalBox::Slot()
				.MaxHeight(50.0f)
				.Padding(DetailsMargin)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("RotationalVariance", "Rotational Variance"))
				]
			]

			+ SSplitter::Slot()
			.Value(0.45f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.MaxHeight(50.0f)
				.Padding(DetailsMargin)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinSliderValue(1)
					.MaxSliderValue(100)
					.MinValue(1)
					.MaxValue(100)
					.Value(this, &SLevelDesignerDesignModeWidget::GetNumBuildingClasses)
					.OnValueChanged(this, &SLevelDesignerDesignModeWidget::SetNumBuildingClasses)
				]

				+ SVerticalBox::Slot()
				.MaxHeight(50.0f)
				.Padding(DetailsMargin)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(ButtonMargin)
					[
						SNew(SNumericEntryBox<float>)
						.AllowSpin(true)
						.MinSliderValue(1.0f)
						.MaxSliderValue(10000.0f)
						.MinValue(1.0f)
						.MaxValue(10000.0f)
						.Value(this, &SLevelDesignerDesignModeWidget::GetDefaultAlleySpacingX)
						.OnValueChanged(this, &SLevelDesignerDesignModeWidget::SetDefaultAlleySpacingX)
					]
					+ SHorizontalBox::Slot()
					.Padding(ButtonMargin)
					[
						SNew(SNumericEntryBox<float>)
						.AllowSpin(true)
						.MinSliderValue(1.0f)
						.MaxSliderValue(10000.0f)
						.MinValue(1.0f)
						.MaxValue(10000.0f)
						.Value(this, &SLevelDesignerDesignModeWidget::GetDefaultAlleySpacingY)
						.OnValueChanged(this, &SLevelDesignerDesignModeWidget::SetDefaultAlleySpacingY)
					]					
				]

				+ SVerticalBox::Slot()
				.MaxHeight(50.0f)
				.Padding(DetailsMargin)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SNumericEntryBox<float>)
					.AllowSpin(true)
					.MinSliderValue(0.0f)
					.MaxSliderValue(360.0f)
					.MinValue(0.0f)
					.MaxValue(360.0f)
					.Value(this, &SLevelDesignerDesignModeWidget::GetRotationalVariance)
					.OnValueChanged(this, &SLevelDesignerDesignModeWidget::SetRotationalVariance)
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

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SelectorBox
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			WidgetSwitcher
		]
	];
}


int32 SLevelDesignerDesignModeWidget::GetSelectedPanel() const
{
	return mCurrentlySelectedBuildingClass;
}

bool SLevelDesignerDesignModeWidget::IsBuildingClassEnabled(int32 classId) const
{
	return mCurrentlySelectedBuildingClass != classId;
}

FReply SLevelDesignerDesignModeWidget::SetCurrentBuildingClass(int32 classId)
{
	mCurrentlySelectedBuildingClass = classId;

	return FReply::Handled();
}

TOptional<int32> SLevelDesignerDesignModeWidget::GetNumBuildingClasses() const
{
	return WidgetTool->GetNumBuildingClasses();
}

void SLevelDesignerDesignModeWidget::SetNumBuildingClasses(int32 value)
{
	WidgetTool->SetNumBuildingClasses(value);
}

TOptional<float> SLevelDesignerDesignModeWidget::GetDefaultAlleySpacingX() const
{
	return WidgetTool->GetDefaultAlleySpacing().X;
}

void SLevelDesignerDesignModeWidget::SetDefaultAlleySpacingX(float value)
{
	FVector2D defaultAlleySpacing = WidgetTool->GetDefaultAlleySpacing();
	defaultAlleySpacing.X = value;

	WidgetTool->SetDefaultAlleySpacing(defaultAlleySpacing);
}

TOptional<float> SLevelDesignerDesignModeWidget::GetDefaultAlleySpacingY() const
{
	return WidgetTool->GetDefaultAlleySpacing().Y;
}

void SLevelDesignerDesignModeWidget::SetDefaultAlleySpacingY(float value)
{
	FVector2D defaultAlleySpacing = WidgetTool->GetDefaultAlleySpacing();
	defaultAlleySpacing.Y = value;

	WidgetTool->SetDefaultAlleySpacing(defaultAlleySpacing);
}

TOptional<float> SLevelDesignerDesignModeWidget::GetRotationalVariance() const
{
	return WidgetTool->GetRotationalVariance();
}

void SLevelDesignerDesignModeWidget::SetRotationalVariance(float value)
{
	WidgetTool->SetRotationalVariance(value);
}