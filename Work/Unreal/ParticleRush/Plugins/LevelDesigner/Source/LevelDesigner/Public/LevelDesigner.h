// Some copyright should be here...

#pragma once

#include "ModuleManager.h"



class FToolBarBuilder;
class FMenuBuilder;

class FLevelDesignerModule : public IModuleInterface
{
public:
	void CreateLevelDesignerAsset();

	UObject* GetLevelDesignerSettingsAsset() { return LevelDesigner_SettingsAsset; }

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:
	UObject* LevelDesigner_SettingsAsset;

	TSharedPtr<class FUICommandList> PluginCommands;
};