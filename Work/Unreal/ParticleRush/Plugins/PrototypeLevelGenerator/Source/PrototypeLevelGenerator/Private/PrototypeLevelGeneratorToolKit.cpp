#include "PrototypeLevelGeneratorPrivatePCH.h"

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Editor/PropertyEditor/Public/IDetailsView.h"
#include "PrototypeLevelGeneratorToolKit.h"
#include "PrototypeLevelGeneratorEdMode.h"

#define LOCTEXT_NAMESPACE "PrototypeLevelGeneratorToolKit"

void SLevelGeneratorModeControls::Construct(const FArguments& InArgs)
{
	//FModeTool* GeometryModeTool = GetModeTool();
	//
	//if (GetModeTool()->GetNumModifiers() > 0)
	//{
	//	GetModeTool()->SetCurrentModifier(GetModeTool()->GetModifier(0));
	//}

	CreateLayout();
}

void SLevelGeneratorModeControls::CreateLayout()
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


FModeTool* SLevelGeneratorModeControls::GetModeTool() const
{
	FPrototypeLevelGeneratorEdMode* Mode = (FPrototypeLevelGeneratorEdMode*)GLevelEditorModeTools().GetActiveMode(FPrototypeLevelGeneratorEdMode::EM_PrototypeLevelGenerator);
	FModeTool* Tool = Mode? Mode->GetCurrentTool(): NULL;

	check(Tool);

	return (FModeTool*)Tool;
}

void FPrototypeLevelGeneratorToolKit::RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FPrototypeLevelGeneratorToolKit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FPrototypeLevelGeneratorToolKit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	LevelGeneratorWidget = SNew(SLevelGeneratorModeControls);

	FModeToolkit::Init(InitToolkitHost);
}

FName FPrototypeLevelGeneratorToolKit::GetToolkitFName() const
{
	return FName("LevelGeneratorMode");
}

FText FPrototypeLevelGeneratorToolKit::GetBaseToolkitName() const
{
	return LOCTEXT( "ToolkitName", "Level Generator Mode" );
}

class FEdMode* FPrototypeLevelGeneratorToolKit::GetEditorMode() const
{
	return (FPrototypeLevelGeneratorEdMode*)GLevelEditorModeTools().GetActiveMode(FPrototypeLevelGeneratorEdMode::EM_PrototypeLevelGenerator);
}


TSharedPtr<SWidget> FPrototypeLevelGeneratorToolKit::GetInlineContent() const
{
	return SNew(SLevelGeneratorModeControls);
}

#undef LOCTEXT_NAMESPACE