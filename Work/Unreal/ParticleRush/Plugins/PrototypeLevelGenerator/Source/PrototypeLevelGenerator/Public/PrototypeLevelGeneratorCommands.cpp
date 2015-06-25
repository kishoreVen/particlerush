// Some copyright should be here...

#include "PrototypeLevelGeneratorPrivatePCH.h"
#include "PrototypeLevelGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FPrototypeLevelGeneratorModule"

void FPrototypeLevelGeneratorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "PrototypeLevelGenerator", "Bring up PrototypeLevelGenerator window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
