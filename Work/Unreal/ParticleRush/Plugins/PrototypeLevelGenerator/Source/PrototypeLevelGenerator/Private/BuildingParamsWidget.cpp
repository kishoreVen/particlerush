#include "PrototypeLevelGeneratorPrivatePCH.h"

#include "BuildingParamsWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "AssetThumbnail.h"
#include "SAssetDropTarget.h"

#define LOCTEXT_NAMESPACE "BuildingParamsWidget"

BuildingParamsWidget::BuildingParamsWidget()
	: BuildingMeshPtr(NULL),
	  BSPBuildingColor(FColorList::Blue),
	  DefaultBuildingMesh("/Game/Assets/Prototype/DefaultBuildingMesh")
{
	
}

BuildingParamsWidget::~BuildingParamsWidget()
{
	AssetThumbnailPool->ReleaseResources();
}


void BuildingParamsWidget::Construct(const FArguments& InArgs)
{
	static const FVector2D ThumbnailSize(64.0f, 64.0f);
	static const FVector2D ColorBlockSize(32.0f, 32.0f);

	/* Create Local Cache */
	BSPBuildingColor = InArgs._BuildingColor;
	DefaultBuildingMeshPtr = LoadMeshFromPath(DefaultBuildingMesh);

	/* Setting up Asset Widget */
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(1024, InArgs._AreRealTimeThumbnailsAllowed));

	/* Check to see if the asset has already not been set */
	if (BuildingMeshPtr.Get() == NULL)
	{
		BuildingMeshPtr = DefaultBuildingMeshPtr;
	}

	const UStaticMesh* staticMesh = BuildingMeshPtr.Get();
	
	AssetThumbnail = MakeShareable(new FAssetThumbnail(staticMesh, ThumbnailSize.X, ThumbnailSize.Y, AssetThumbnailPool));

	TSharedRef<SWidget> AssetWidget =
		SNew(SAssetDropTarget)
		.OnIsAssetAcceptableForDrop(this, &BuildingParamsWidget::IsAssetAcceptableForDrop)
		.OnAssetDropped(this, &BuildingParamsWidget::SetBuildingMesh)
		[
			SNew(SBox)
			.WidthOverride(ThumbnailSize.X)
			.HeightOverride(ThumbnailSize.Y)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(5)
			[
				AssetThumbnail->MakeThumbnailWidget()
			]
		];

	const TSharedRef<SWidget> UseButton = PropertyCustomizationHelpers::MakeUseSelectedButton(FSimpleDelegate::CreateSP(this, &BuildingParamsWidget::OnUseSelectedStaticMesh));
	UseButton->SetEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &BuildingParamsWidget::CanUseSelectedAsset)));

	const TSharedRef<SWidget> BrowseButton = PropertyCustomizationHelpers::MakeBrowseButton(FSimpleDelegate::CreateSP(this, &BuildingParamsWidget::OnBrowseToStaticMesh));
	BrowseButton->SetEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &BuildingParamsWidget::CanBrowseToAsset)));

	const TSharedRef<SWidget> ClearButton = PropertyCustomizationHelpers::MakeClearButton(FSimpleDelegate::CreateSP(this, &BuildingParamsWidget::OnClearStaticMesh));
	ClearButton->SetEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &BuildingParamsWidget::CanUseClearButton)));

	TSharedRef<SWidget> ButtonsColumnWidget =
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(1.0f)
		.AutoHeight()
		[
			UseButton
		]
	+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(1.0f)
		.AutoHeight()
		[
			BrowseButton
		];

	TSharedRef<SWidget> ClearColumnWidget =
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(1.0f)
		.AutoHeight()
		[
			ClearButton
		];

	TSharedRef<SWidget> ThumbnailWidget =
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(1.0f)
		.AutoHeight()
		[
			AssetWidget
		];

	TSharedRef<SWidget> ColorBlockWidget =
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.Padding(1.0f)
		.MaxHeight(ThumbnailSize.Y)
		[
			SNew(SColorBlock)
			.Color(InArgs._BuildingColor)
			.Size(ColorBlockSize)
			.ToolTipText(LOCTEXT("BuildingMeshColor", "The Color for this class of Building Mesh"))
		];

	ChildSlot
		[
			SNew(SSplitter)

			+ SSplitter::Slot()
			.Value(0.2f)
			[
				ColorBlockWidget
			]

			+ SSplitter::Slot()
			.Value(0.8f)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.FillWidth(0.8f)
				.Padding(FMargin(5, 0, 5, 0))
				[
					SAssignNew(ComboButton, SComboButton)
					.ToolTipText(this, &BuildingParamsWidget::OnGetToolTip)
					.ButtonStyle(FEditorStyle::Get(), "PropertyEditor.AssetComboStyle")
					.ForegroundColor(FEditorStyle::GetColor("PropertyEditor.AssetName.ColorAndOpacity"))
					.OnGetMenuContent(this, &BuildingParamsWidget::OnGetMenuContent)
					.ContentPadding(2.0f)
					.ButtonContent()
					[
						SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.VAlign(EVerticalAlignment::VAlign_Center)
						.FillWidth(0.3f)
						.Padding(FMargin(5, 0, 5, 0))
						[
							ThumbnailWidget
						]

						+ SHorizontalBox::Slot()
						.VAlign(EVerticalAlignment::VAlign_Center)
						.FillWidth(0.5f)
						.Padding(FMargin(5, 0, 5, 0))
						[
							SNew(STextBlock)
							.Text(this, &BuildingParamsWidget::GetSelectedAssetName)
							.ToolTipText(this, &BuildingParamsWidget::GetSelectedAssetPath)
						]
					]
				]

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.FillWidth(0.1f)
				.Padding(FMargin(5, 0, 5, 0))
				[
					ButtonsColumnWidget
				]

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.FillWidth(0.1f)
				.Padding(FMargin(5, 0, 5, 0))
				[
					ClearColumnWidget
				]
			]
		];
}

void BuildingParamsWidget::SetBuildingMesh(UObject* NewAsset)
{
	if (UStaticMesh* NewStaticMesh = Cast<UStaticMesh>(NewAsset))
	{
		if (BuildingMeshPtr.Get() != NewStaticMesh)
		{
			BuildingMeshPtr = NewStaticMesh;
			AssetThumbnail->SetAsset(BuildingMeshPtr.Get());
		}
	}
}


FText BuildingParamsWidget::OnGetToolTip() const
{
	return LOCTEXT("SelectMesh", "Select the corresponding building mesh");
}


TSharedRef<SWidget> BuildingParamsWidget::OnGetMenuContent()
{
	TArray<const UClass*> AllowedClasses;
	AllowedClasses.Add(UStaticMesh::StaticClass());


	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		BuildingMeshPtr.Get(),
		false,
		AllowedClasses,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(AllowedClasses),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateSP(this, &BuildingParamsWidget::OnAssetSelectedFromPicker),
		FSimpleDelegate::CreateSP(this, &BuildingParamsWidget::CloseMenu));
}


void BuildingParamsWidget::CloseMenu()
{
	ComboButton->SetIsOpen(false);
}


void BuildingParamsWidget::OnAssetSelectedFromPicker(const FAssetData& InAssetData)
{
	UStaticMesh* NewBuildingMesh = Cast<UStaticMesh>(InAssetData.GetAsset());
	if (NewBuildingMesh)
		SetBuildingMesh(NewBuildingMesh);
}


void BuildingParamsWidget::OnUseSelectedStaticMesh()
{
	// Load selected assets
	FEditorDelegates::LoadSelectedAssetsIfNeeded.Broadcast();

	// Get the first static mesh selected
	USelection* StaticMeshSelection = GEditor->GetSelectedObjects();
	if (StaticMeshSelection && StaticMeshSelection->Num() == 1)
	{
		UStaticMesh* StaticMeshToAssign = StaticMeshSelection->GetTop<UStaticMesh>();
		if (StaticMeshToAssign)
			SetBuildingMesh(StaticMeshToAssign);
	}
}


bool BuildingParamsWidget::CanUseSelectedAsset()
{
	bool Result = false;

	// Load selected assets
	FEditorDelegates::LoadSelectedAssetsIfNeeded.Broadcast();

	// Get the first static mesh selected
	USelection* StaticMeshSelection = GEditor->GetSelectedObjects();
	if (StaticMeshSelection && StaticMeshSelection->Num() == 1)
	{
		UStaticMesh* StaticMeshToAssign = StaticMeshSelection->GetTop<UStaticMesh>();
		if (StaticMeshToAssign)
		{
			Result = true;
		}
	}

	return Result;
}


void BuildingParamsWidget::OnBrowseToStaticMesh()
{
	UStaticMesh* staticMesh = BuildingMeshPtr.Get();

	if (staticMesh)
	{
		// Find the item in the content browser
		TArray< UObject* > Objects;
		Objects.Add(staticMesh);
		GEditor->SyncBrowserToObjects(Objects);
	}
}


bool BuildingParamsWidget::CanBrowseToAsset()
{
	const UStaticMesh* staticMesh = BuildingMeshPtr.Get();

	return staticMesh != NULL;
}


void BuildingParamsWidget::OnClearStaticMesh()
{
	UStaticMesh* staticMesh = BuildingMeshPtr.Get();
	UStaticMesh* defaultMesh = DefaultBuildingMeshPtr.Get();

	if (staticMesh != NULL && staticMesh != defaultMesh)
	{
		SetBuildingMesh(defaultMesh);
	}
}


bool BuildingParamsWidget::CanUseClearButton()
{
	UStaticMesh* staticMesh = BuildingMeshPtr.Get();
	UStaticMesh* defaultMesh = DefaultBuildingMeshPtr.Get();

	return staticMesh != defaultMesh;
}


FText BuildingParamsWidget::GetSelectedAssetName() const
{
	UStaticMesh* staticMesh = BuildingMeshPtr.Get();

	if (staticMesh == NULL)
		return LOCTEXT("None", "No Asset Selected");

	return FText::FromString(staticMesh->GetName());
}


FText BuildingParamsWidget::GetSelectedAssetPath() const
{
	UStaticMesh* staticMesh = BuildingMeshPtr.Get();

	if (staticMesh == NULL)
		return LOCTEXT("None", "No Asset Selected");

	return FText::FromString(staticMesh->GetPathName());
}


bool BuildingParamsWidget::IsDefaultMesh()
{
	UStaticMesh* staticMesh = BuildingMeshPtr.Get();
	UStaticMesh* defaultMesh = DefaultBuildingMeshPtr.Get();

	return staticMesh == defaultMesh;
}