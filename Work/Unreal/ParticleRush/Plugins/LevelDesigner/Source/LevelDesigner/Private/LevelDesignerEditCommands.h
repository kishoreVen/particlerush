
#pragma once

/**
 * 
 */
class FLevelDesignerEditCommands : public TCommands<FLevelDesignerEditCommands>
{

public:
	FLevelDesignerEditCommands() : TCommands<FLevelDesignerEditCommands>
	(
		"FLevelDesignerEditCommands", // Context name for fast lookup
		NSLOCTEXT("Contexts", "LevelDesignerEditCommands", "Level Designer Edit Commands"), // Localized context name for displaying
		NAME_None, // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
	{
	}
	
	
	/** Commands for the brush settings. */
	TSharedPtr< FUICommandInfo > IncreaseBrushSize;
	TSharedPtr< FUICommandInfo > DecreaseBrushSize;

	/** Commands for the tools toolbar. */
	TSharedPtr< FUICommandInfo > EraseMode;
	TSharedPtr< FUICommandInfo > DesignMode;

	virtual void RegisterCommands() override;

public:

	TMap<FName, TSharedPtr<FUICommandInfo>> NameToCommandMap;

	static FName EraseModeName;
	static FName DesignModeName;
};
