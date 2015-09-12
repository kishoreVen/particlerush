// Some copyright should be here...

#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerStyle.h"
#include "LevelDesignerCommands.h"
#include "LevelDesignerEdMode.h"
#include "LevelDesignerAsset.h"

#include "AssetToolsModule.h"

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

	CreateLevelDesignerAsset();
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

void FLevelDesignerModule::CreateLevelDesignerAsset()
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

	FString Name;
	FString PackageName;
	const FString PathPrefix = TEXT("/Game/");
	const FString AssetName = TEXT("LevelDesigner");
	const FString DefaultSuffix = TEXT("_SettingsAsset");

	AssetToolsModule.Get().CreateUniqueAssetName(AssetName, DefaultSuffix, /*out*/ PackageName, /*out*/ Name);
	const FString PackagePath = PathPrefix + FPackageName::GetLongPackagePath(PackageName);

	LevelDesigner_SettingsAsset = StaticLoadObject(ULevelDesignerAsset::StaticClass(), NULL, *(PackagePath + TEXT("/") + Name));

	if (LevelDesigner_SettingsAsset == NULL)
	{
		UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PackagePath, ULevelDesignerAsset::StaticClass(), NULL);

		LevelDesigner_SettingsAsset = NewAsset;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLevelDesignerModule, LevelDesigner)