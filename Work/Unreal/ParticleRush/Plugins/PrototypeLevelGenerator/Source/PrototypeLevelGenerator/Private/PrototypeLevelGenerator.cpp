// Some copyright should be here...

#include "PrototypeLevelGeneratorPrivatePCH.h"


#include "PrototypeLevelGeneratorStyle.h"
#include "PrototypeLevelGeneratorCommands.h"
#include "PrototypeLevelGeneratorWindow.h"


static const FName PrototypeLevelGeneratorTabName("PrototypeLevelGenerator");

#define LOCTEXT_NAMESPACE "FPrototypeLevelGeneratorModule"

void FPrototypeLevelGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPrototypeLevelGeneratorStyle::Initialize();
	FPrototypeLevelGeneratorStyle::ReloadTextures();

	FPrototypeLevelGeneratorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPrototypeLevelGeneratorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPrototypeLevelGeneratorModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FPrototypeLevelGeneratorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FPrototypeLevelGeneratorModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PrototypeLevelGeneratorTabName, FOnSpawnTab::CreateRaw(this, &FPrototypeLevelGeneratorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FPrototypeLevelGeneratorTabTitle", "PrototypeLevelGenerator"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPrototypeLevelGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	FPrototypeLevelGeneratorStyle::Shutdown();

	FPrototypeLevelGeneratorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PrototypeLevelGeneratorTabName);
}

TSharedRef<SDockTab> FPrototypeLevelGeneratorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.Label(LOCTEXT("TabTitle", "PrototypeLevelGeneratorTab"))
		.TabRole(ETabRole::NomadTab)
		.ContentPadding(5)
		[
			SNew(SBorder)
			.Padding(4)
			.BorderImage(FEditorStyle::GetBrush("Docking.Tab.ContentAreaBrush"))
			[
				SNew(PrototypeLevelGeneratorWindow)
			]
		];
}

void FPrototypeLevelGeneratorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(PrototypeLevelGeneratorTabName);
}

void FPrototypeLevelGeneratorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FPrototypeLevelGeneratorCommands::Get().OpenPluginWindow);
}

void FPrototypeLevelGeneratorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FPrototypeLevelGeneratorCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPrototypeLevelGeneratorModule, PrototypeLevelGenerator)