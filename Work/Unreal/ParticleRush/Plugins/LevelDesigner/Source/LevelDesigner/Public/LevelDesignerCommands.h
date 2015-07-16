// Some copyright should be here...

#pragma once

#include "SlateBasics.h"
#include "LevelDesignerStyle.h"

class FLevelDesignerCommands : public TCommands<FLevelDesignerCommands>
{
public:

	FLevelDesignerCommands()
		: TCommands<FLevelDesignerCommands>(TEXT("LevelDesigner"), NSLOCTEXT("Contexts", "LevelDesigner", "LevelDesigner Plugin"), NAME_None, FLevelDesignerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
