// Some copyright should be here...

#pragma once

#include "ModuleManager.h"



class FToolBarBuilder;
class FMenuBuilder;

class FPrototypeLevelGeneratorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	///** This function will be bound to Command (by default it will bring up plugin window) */
	//void PluginButtonClicked();
	
private:

	//void AddToolbarExtension(FToolBarBuilder& Builder);
	//void AddMenuExtension(FMenuBuilder& Builder);
	//
	//TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	//TSharedPtr<class FUICommandList> PluginCommands;
};