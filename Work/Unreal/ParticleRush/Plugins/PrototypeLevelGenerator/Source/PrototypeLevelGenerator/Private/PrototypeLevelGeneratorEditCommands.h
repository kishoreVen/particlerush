
#pragma once

/**
 * 
 */
class FPrototypeLevelGeneratorEditCommands : public TCommands<FPrototypeLevelGeneratorEditCommands>
{

public:
	FPrototypeLevelGeneratorEditCommands() : TCommands<FPrototypeLevelGeneratorEditCommands>
	(
		"PrototypeLevelGeneratorEditCommands", // Context name for fast lookup
		NSLOCTEXT("Contexts", "PrototypeLevelGeneratorEditCommands", "Prototype Level Generator Edit Commands"), // Localized context name for displaying
		NAME_None, // Parent
		FEditorStyle::GetStyleSetName() // Icon Style Set
	)
	{
	}
	
	
	/** Commands for the foliage brush settings. */
	TSharedPtr< FUICommandInfo > IncreaseBrushSize;
	TSharedPtr< FUICommandInfo > DecreaseBrushSize;

	/** Commands for the tools toolbar. */
	TSharedPtr< FUICommandInfo > SetPaint;

	virtual void RegisterCommands() override;

public:
};
