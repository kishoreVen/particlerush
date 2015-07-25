// Some copyright should be here...
#pragma once

#include "SlateBasics.h"

/**  */
class FLevelDesignerStyle
{
public:

	static void Initialize();

	static void Shutdown();

	/** reloads textures used by slate renderer */
	static void ReloadTextures();

	/** @return The Slate style set for the Shooter game */
	static const ISlateStyle& Get();

	static const FSlateBrush* GetBrush(FName PropertyName, const ANSICHAR* Specifier = NULL);

	static FName GetStyleSetName();

private:

	static TSharedRef< class FSlateStyleSet > Create();

private:

	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};