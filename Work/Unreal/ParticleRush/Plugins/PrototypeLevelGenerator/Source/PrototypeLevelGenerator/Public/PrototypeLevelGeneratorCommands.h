// Some copyright should be here...

#pragma once

#include "SlateBasics.h"
#include "PrototypeLevelGeneratorStyle.h"

class FPrototypeLevelGeneratorCommands : public TCommands<FPrototypeLevelGeneratorCommands>
{
public:

	FPrototypeLevelGeneratorCommands()
		: TCommands<FPrototypeLevelGeneratorCommands>(TEXT("PrototypeLevelGenerator"), NSLOCTEXT("Contexts", "PrototypeLevelGenerator", "PrototypeLevelGenerator Plugin"), NAME_None, FPrototypeLevelGeneratorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};