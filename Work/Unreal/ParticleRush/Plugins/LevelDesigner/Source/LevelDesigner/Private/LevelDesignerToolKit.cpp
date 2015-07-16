#include "LevelDesignerPrivatePCH.h"

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Editor/PropertyEditor/Public/IDetailsView.h"
#include "LevelDesignerToolKit.h"
#include "LevelDesignerEdMode.h"

#define LOCTEXT_NAMESPACE "LevelDesignerToolKit"

void SLevelDesignerModeControls::Construct(const FArguments& InArgs)
{
	CreateLayout();
}

void SLevelDesignerModeControls::CreateLayout()
{
	this->ChildSlot
	[
		SNew(SScrollBox)
		+SScrollBox::Slot()
		.Padding(0.0f)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNullWidget::NullWidget
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(3.0f)
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]	
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNullWidget::NullWidget
				]	
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(3.0f)
				[
					SNew(SSeparator)
					.Orientation(Orient_Horizontal)
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNullWidget::NullWidget
				]
			]
		]
	];
}


FModeTool* SLevelDesignerModeControls::GetModeTool() const
{
	FLevelDesignerEdMode* Mode = (FLevelDesignerEdMode*)GLevelEditorModeTools().GetActiveMode(FLevelDesignerEdMode::EM_LevelDesigner);
	FModeTool* Tool = Mode? Mode->GetCurrentTool(): NULL;

	check(Tool);

	return (FModeTool*)Tool;
}

void FLevelDesignerToolKit::RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FLevelDesignerToolKit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FLevelDesignerToolKit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	LevelGeneratorWidget = SNew(SLevelDesignerModeControls);

	FModeToolkit::Init(InitToolkitHost);
}

FName FLevelDesignerToolKit::GetToolkitFName() const
{
	return FName("LevelDesignerMode");
}

FText FLevelDesignerToolKit::GetBaseToolkitName() const
{
	return LOCTEXT( "ToolkitName", "Level Designer Mode" );
}

class FEdMode* FLevelDesignerToolKit::GetEditorMode() const
{
	return (FLevelDesignerEdMode*)GLevelEditorModeTools().GetActiveMode(FLevelDesignerEdMode::EM_LevelDesigner);
}


TSharedPtr<SWidget> FLevelDesignerToolKit::GetInlineContent() const
{
	return SNew(SLevelDesignerModeControls);
}

#undef LOCTEXT_NAMESPACE