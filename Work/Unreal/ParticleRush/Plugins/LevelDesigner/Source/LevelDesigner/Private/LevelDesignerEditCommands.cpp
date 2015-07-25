// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerEditCommands.h"

#define LOCTEXT_NAMESPACE ""


FName FLevelDesignerEditCommands::EraseModeName("ToolMode_Erase");
FName FLevelDesignerEditCommands::DesignModeName("ToolMode_Design");

void FLevelDesignerEditCommands::RegisterCommands()
{
	UI_COMMAND(EraseMode, "EraseMode", "", EUserInterfaceActionType::RadioButton, FInputChord());
	NameToCommandMap.Add(EraseModeName, EraseMode);
	UI_COMMAND(DesignMode, "DesignMode", "", EUserInterfaceActionType::RadioButton, FInputChord());
	NameToCommandMap.Add(DesignModeName, DesignMode);

	UI_COMMAND(DecreaseBrushSize, "Decrease Brush Size", "Decreases the size of the foliage brush", EUserInterfaceActionType::Button, FInputChord(EKeys::LeftBracket));
	UI_COMMAND(IncreaseBrushSize, "Increase Brush Size", "Increases the size of the foliage brush", EUserInterfaceActionType::Button, FInputChord(EKeys::RightBracket));
}

#undef LOCTEXT_NAMESPACE