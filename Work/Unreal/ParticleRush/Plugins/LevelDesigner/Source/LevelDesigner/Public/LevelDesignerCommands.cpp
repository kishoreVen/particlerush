// Some copyright should be here...

#include "LevelDesignerPrivatePCH.h"
#include "LevelDesignerCommands.h"

#define LOCTEXT_NAMESPACE "FLevelDesignerModule"

void FLevelDesignerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "LevelDesigner", "Execute LevelDesigner action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
