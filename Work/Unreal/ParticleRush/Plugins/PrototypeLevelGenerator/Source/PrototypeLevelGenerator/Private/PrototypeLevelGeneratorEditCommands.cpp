// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PrototypeLevelGeneratorPrivatePCH.h"

#include "PrototypeLevelGeneratorEditCommands.h"

#define LOCTEXT_NAMESPACE ""

void FPrototypeLevelGeneratorEditCommands::RegisterCommands()
{
	UI_COMMAND(DecreaseBrushSize, "Decrease Brush Size", "Decreases the size of the foliage brush", EUserInterfaceActionType::Button, FInputChord(EKeys::LeftBracket));
	UI_COMMAND(IncreaseBrushSize, "Increase Brush Size", "Increases the size of the foliage brush", EUserInterfaceActionType::Button, FInputChord(EKeys::RightBracket));

	UI_COMMAND( SetPaint, "Paint", "Paint", EUserInterfaceActionType::ToggleButton, FInputChord() );
}

#undef LOCTEXT_NAMESPACE