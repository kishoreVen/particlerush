// Copyright 2015. Kishore Venkateshan


#include "LevelGeneratorPluginPrivatePCH.h"
#include "LevelGeneratorPlugin.h"


IMPLEMENT_MODULE( FLevelGeneratorPlugin, LevelGeneratorPlugin )

DEFINE_LOG_CATEGORY(LevelGeneratorPluginLog)

#define LOCTEXT_NAMESPACE "LevelGeneratorPlugin"


FLevelGeneratorPlugin::FLevelGeneratorPlugin()
{

}


void FLevelGeneratorPlugin::StartupModule()
{
	/* Create a delegations for menu when the window spawns */
	TSharedPtr<FExtender> extender = MakeShareable(new FExtender);
	extender->AddMenuExtension(
		"WindowLayout",
		EExtensionHook::After,
		NULL,
		FMenuExtensionDelegate::CreateRaw(this, &FLevelGeneratorPlugin::CreateWindowMenu)
		);

	/* Extend the level editor */
	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetMenuExtensibilityManager()->AddExtender(extender);

	UE_LOG(LevelGeneratorPluginLog, Warning, TEXT("Loaded Level Generator Plugin"));
}


void FLevelGeneratorPlugin::ShutdownModule()
{
	UE_LOG(LevelGeneratorPluginLog, Warning, TEXT("Unloaded Level Generator Plugin"));
}


void FLevelGeneratorPlugin::OpenPluginWindow()
{
	UE_LOG(LevelGeneratorPluginLog, Warning, TEXT("Loaded Level Generator Plugin Window"));
}


void FLevelGeneratorPlugin::CreateWindowMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("OpenWindow", "Level Generator Plugin Window"),
		LOCTEXT("OpenWindowToolTip", "Loads the level generator plugin to quickly populate a world with prototype meshes"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FLevelGeneratorPlugin::OpenPluginWindow))
		);
}