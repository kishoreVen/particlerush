
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
	
	
	/** Commands for the foliage brush settings. */
	TSharedPtr< FUICommandInfo > IncreaseBrushSize;
	TSharedPtr< FUICommandInfo > DecreaseBrushSize;

	/** Commands for the tools toolbar. */
	TSharedPtr< FUICommandInfo > SetErase;

	virtual void RegisterCommands() override;

public:
};
