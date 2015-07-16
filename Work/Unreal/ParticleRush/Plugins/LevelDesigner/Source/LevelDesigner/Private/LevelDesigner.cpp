// Some copyright should be here...

#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerStyle.h"
#include "LevelDesignerCommands.h"
#include "LevelDesignerEdMode.h"


static const FName LevelDesignerTabName("LevelDesigner");

#define LOCTEXT_NAMESPACE "FLevelDesignerModule"

void FLevelDesignerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLevelDesignerStyle::Initialize();
	FLevelDesignerStyle::ReloadTextures();

	FLevelDesignerCommands::Register();

	FEditorModeRegistry::Get().RegisterMode<FLevelDesignerEdMode>(
		FLevelDesignerEdMode::EM_LevelDesigner,
		LOCTEXT("LevelDesignerEditMode", "Level Designer Editor"),
		FSlateIcon(),
		true);
}

void FLevelDesignerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FEditorModeRegistry::Get().UnregisterMode(FLevelDesignerEdMode::EM_LevelDesigner);
	
	FLevelDesignerStyle::Shutdown();

	FLevelDesignerCommands::Unregister();
}

void FLevelDesignerModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
}

void FLevelDesignerModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FLevelDesignerCommands::Get().PluginAction);
}

void FLevelDesignerModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FLevelDesignerCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLevelDesignerModule, LevelDesigner)