// Some copyright should be here...
#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerStyle.h"
#include "SlateGameResources.h"
#include "IPluginManager.h"

TSharedPtr< FSlateStyleSet > FLevelDesignerStyle::StyleInstance = NULL;

TSharedRef< FLinearColor > FLevelDesignerStyle::SelectionColor_LinearRef(MakeShareable(new FLinearColor(0.728f, 0.364f, 0.003f)));
TSharedRef< FLinearColor > FLevelDesignerStyle::SelectionColor_Pressed_LinearRef(MakeShareable(new FLinearColor(0.701f, 0.225f, 0.003f)));

FSlateColor FLevelDesignerStyle::SelectionColor(SelectionColor_LinearRef);
FSlateColor FLevelDesignerStyle::SelectionColor_Pressed(SelectionColor_Pressed_LinearRef);

void FLevelDesignerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FLevelDesignerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FLevelDesignerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("LevelDesignerStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FLevelDesignerStyle::Create()
{
	static FVector2D Icon64x64(64.0f, 64.0f);

	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("LevelDesignerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("LevelDesigner")->GetBaseDir() / TEXT("Resources"));

	Style->Set("LevelDesigner.PluginAction",	new IMAGE_BRUSH(TEXT("ButtonIcon_40x"),			Icon40x40));
	Style->Set("LevelDesigner.DesignMode",		new IMAGE_BRUSH(TEXT("Icon_Design"),			Icon40x40));
	Style->Set("LevelDesigner.EraseMode",		new IMAGE_BRUSH(TEXT("Icon_Erase"),				Icon40x40));
	Style->Set("LevelDesigner.DesignDelete",	new IMAGE_BRUSH(TEXT("Icon_DeleteEverything"),	Icon40x40));
	Style->Set("LevelDesigner.BoxTrigger",		new IMAGE_BRUSH(TEXT("Icon_BoxTrigger"),		Icon40x40));

	Style->Set("LevelDesigner.ToggleButton", FButtonStyle()
		.SetNormal(FSlateNoResource())
		.SetHovered(BOX_BRUSH(TEXT("RoundedSelection_16x"), 4.0f / 16.0f, SelectionColor))
		.SetPressed(BOX_BRUSH("RoundedSelection_16x", 4.0f / 16.0f, SelectionColor_Pressed))
		.SetDisabled(BOX_BRUSH("RoundedSelection_16x", 4.0f / 16.0f, SelectionColor))
		.SetNormalPadding(FMargin(2, 2, 2, 2))
		.SetPressedPadding(FMargin(2, 3, 2, 1))
		);
	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FLevelDesignerStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FLevelDesignerStyle::Get()
{
	return *StyleInstance;
}

const FSlateBrush* FLevelDesignerStyle::GetBrush(FName PropertyName, const ANSICHAR* Specifier)
{
	return StyleInstance->GetBrush(PropertyName, Specifier);
}
