#include "LevelDesignerPrivatePCH.h"

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Editor/PropertyEditor/Public/IDetailsView.h"
#include "LevelDesignerToolKit.h"
#include "LevelDesignerEdMode.h"

#include "LevelDesignerEditCommands.h"
#include "LevelDesignerStyle.h"

#include "LevelDesignerTools.h"

#define LOCTEXT_NAMESPACE "LevelDesignerToolKit"

void SLevelDesignerModeControls::Construct(const FArguments& InArgs, TSharedRef<FLevelDesignerToolKit> InParentToolkit)
{
	ParentToolKit = InParentToolkit;

	CommandList = ParentToolKit->GetToolkitCommands();

	CreateLayout(InArgs);
}

void SLevelDesignerModeControls::CreateLayout(const FArguments& InArgs)
{
	ContentWidget = SNew(SVerticalBox);

	CommandList->MapAction(
		FLevelDesignerEditCommands::Get().EraseMode,
		FUIAction(
			FExecuteAction::CreateSP(this, &SLevelDesignerModeControls::OnChangeMode, ELevelDesignerModeTools::MT_Erase),
			FCanExecuteAction::CreateSP(this, &SLevelDesignerModeControls::IsModeEnabled, ELevelDesignerModeTools::MT_Erase),
			FIsActionChecked::CreateSP(this, &SLevelDesignerModeControls::IsModeActive, ELevelDesignerModeTools::MT_Erase)
			)
		);
	
	CommandList->MapAction(
		FLevelDesignerEditCommands::Get().DesignMode,
		FUIAction(
		FExecuteAction::CreateSP(this, &SLevelDesignerModeControls::OnChangeMode, ELevelDesignerModeTools::MT_Design),
			FCanExecuteAction::CreateSP(this, &SLevelDesignerModeControls::IsModeEnabled, ELevelDesignerModeTools::MT_Design),
			FIsActionChecked::CreateSP(this, &SLevelDesignerModeControls::IsModeActive, ELevelDesignerModeTools::MT_Design)
			)
		);

	FToolBarBuilder ModeSwitchButtons(CommandList, FMultiBoxCustomization::None);
	{
		ModeSwitchButtons.AddToolBarButton(
			FLevelDesignerEditCommands::Get().EraseMode, 
			NAME_None, 
			LOCTEXT("Mode.Erase", "Erase"), 
			LOCTEXT("Mode.Erase.Tooltip", "Tool to remove unwanted objects"), 
			FSlateIcon(FLevelDesignerStyle::GetStyleSetName(), TEXT("LevelDesigner.EraseMode"))
		);

		ModeSwitchButtons.AddToolBarButton(
			FLevelDesignerEditCommands::Get().DesignMode,
			NAME_None, 
			LOCTEXT("Mode.Design", "Design"), 
			LOCTEXT("Mode.Design.Tooltip", "Contains tools to design the level"), 
			FSlateIcon(FLevelDesignerStyle::GetStyleSetName(), TEXT("LevelDesigner.DesignMode"))
		);
	}

	TSharedRef<SWidget> toolBoxWidget = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			.HAlign(HAlign_Center)
			[

				ModeSwitchButtons.MakeWidget()
			]
		];
		
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
					toolBoxWidget
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
					ContentWidget.ToSharedRef()
				]
			]
		]
	];

	UpdateContentWidget();
}

void SLevelDesignerModeControls::UpdateContentWidget()
{
	ContentWidget->ClearChildren();

	FLevelDesignerEdMode* levelDesignerEdMode = (FLevelDesignerEdMode*)ParentToolKit->GetEditorMode();
	if (levelDesignerEdMode)
	{
		ContentWidget->AddSlot()
			.Padding(10)
			[
				((FLevelDesigner_BaseModeTool*)(levelDesignerEdMode->GetCurrentTool()))->MakeWidget()
			];
	}
}

void SLevelDesignerModeControls::OnChangeMode(ELevelDesignerModeTools ModeID)
{
	FLevelDesignerEdMode* levelDesignerEdMode = (FLevelDesignerEdMode*)ParentToolKit->GetEditorMode();
	if (levelDesignerEdMode)
	{
		levelDesignerEdMode->SetCurrentLevelDesignerTool(ModeID);
	
		UpdateContentWidget();
	}
}

bool SLevelDesignerModeControls::IsModeEnabled(ELevelDesignerModeTools ModeID) const
{
	return true;
}

bool SLevelDesignerModeControls::IsModeActive(ELevelDesignerModeTools ModeID) const
{
	FLevelDesignerEdMode* levelDesignerEdMode = (FLevelDesignerEdMode*)ParentToolKit->GetEditorMode();
	if (levelDesignerEdMode)
	{
		return (levelDesignerEdMode->GetCurrentTool()->GetID() == ModeID);
	}

	return false;
}

void FLevelDesignerToolKit::RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FLevelDesignerToolKit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager)
{

}

void FLevelDesignerToolKit::Init(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	LevelGeneratorWidget = SNew(SLevelDesignerModeControls, SharedThis(this));

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
	return GLevelEditorModeTools().GetActiveMode(FLevelDesignerEdMode::EM_LevelDesigner);
}

TSharedPtr<SWidget> FLevelDesignerToolKit::GetInlineContent() const
{
	return LevelGeneratorWidget;
}

#undef LOCTEXT_NAMESPACE