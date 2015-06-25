// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LevelGeneratorPluginLog, Log, All)

class FLevelGeneratorPlugin : public ILevelGeneratorPlugin
{
private:
	void OpenPluginWindow();

	void CreateWindowMenu(FMenuBuilder& MenuBuilder);

public:
	FLevelGeneratorPlugin();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};