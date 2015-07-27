#include "LevelDesignerPrivatePCH.h"

#include "LevelDesignerBuildingClass.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetThumbnail.h"
#include "SAssetDropTarget.h"

/*--------------------------
Level Designer Building Class
----------------------------*/

FLevelDesignerBuilding::FLevelDesignerBuilding(const FColor& buildingClassColor)
: SpawnableBuildingPtr(NULL)
, BuildingClassColor(buildingClassColor)
{
	
}

FLevelDesignerBuilding::~FLevelDesignerBuilding()
{
	
}

TSharedRef<SLevelDesignerBuildingWidget> FLevelDesignerBuilding::MakeWidget()
{
	return SNew(SLevelDesignerBuildingWidget, this);
}

TSharedRef<SColorBlock> FLevelDesignerBuilding::MakeColorBlockWidget()
{
	static const FVector2D ColorBlockSize(32.0f, 32.0f);

	TSharedRef<SColorBlock> widget = SNew(SColorBlock)
									.Color(BuildingClassColor)
									.Size(ColorBlockSize);

	return widget;
}

void FLevelDesignerBuilding::SetBuildingObject(UObject* NewAsset)
{
	SpawnableBuildingPtr = NewAsset;
}

UObject* FLevelDesignerBuilding::GetBuildingObject() const
{
	return SpawnableBuildingPtr;
}


/*--------------------------
Level Designer Building Widget
----------------------------*/

SLevelDesignerBuildingWidget::SLevelDesignerBuildingWidget()
{
	
}

SLevelDesignerBuildingWidget::~SLevelDesignerBuildingWidget()
{
	
}

void SLevelDesignerBuildingWidget::Construct(const FArguments& InArgs, FLevelDesignerBuilding* InLevelDesignerBuilding)
{
	LevelDesignerBuilding = InLevelDesignerBuilding;

	static const FVector2D ThumbnailSize(64.0f, 64.0f);

	const UObject* BuildingObject = NULL;

	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(1024, true));
	AssetThumbnail = MakeShareable(new FAssetThumbnail(BuildingObject, ThumbnailSize.X, ThumbnailSize.Y, AssetThumbnailPool));

	TSharedRef<SWidget> AssetWidget =
	SNew(SAssetDropTarget)
	.OnIsAssetAcceptableForDrop(this, &SLevelDesignerBuildingWidget::IsAssetAcceptableForDrop)
	.OnAssetDropped(this, &SLevelDesignerBuildingWidget::SetBuildingObject)
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

	const TSharedRef<SWidget> UseButton = PropertyCustomizationHelpers::MakeUseSelectedButton(FSimpleDelegate::CreateSP(this, &SLevelDesignerBuildingWidget::OnUseSelectedAsset));
	UseButton->SetEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &SLevelDesignerBuildingWidget::CanUseSelectedAsset)));

	const TSharedRef<SWidget> BrowseButton = PropertyCustomizationHelpers::MakeBrowseButton(FSimpleDelegate::CreateSP(this, &SLevelDesignerBuildingWidget::OnBrowseToAsset));
	BrowseButton->SetEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &SLevelDesignerBuildingWidget::CanBrowseToAsset)));

	const TSharedRef<SWidget> ClearButton = PropertyCustomizationHelpers::MakeClearButton(FSimpleDelegate::CreateSP(this, &SLevelDesignerBuildingWidget::OnClearAsset));
	ClearButton->SetEnabled(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateSP(this, &SLevelDesignerBuildingWidget::CanUseClearButton)));
	
	
	TSharedRef<SWidget> ButtonsColumnWidget =
	SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.VAlign(EVerticalAlignment::VAlign_Center)
	.HAlign(EHorizontalAlignment::HAlign_Center)
	.Padding(1.0f)
	.AutoHeight()
	[
		UseButton
	]
	+ SVerticalBox::Slot()
	.VAlign(EVerticalAlignment::VAlign_Center)
	.HAlign(EHorizontalAlignment::HAlign_Center)
	.Padding(1.0f)
	.AutoHeight()
	[
		BrowseButton
	]
	+ SVerticalBox::Slot()
	.VAlign(EVerticalAlignment::VAlign_Center)
	.HAlign(EHorizontalAlignment::HAlign_Center)
	.Padding(1.0f)
	.AutoHeight()
	[
		ClearButton
	];

	ChildSlot
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SAssignNew(ComboButton, SComboButton)
			.ButtonStyle(FEditorStyle::Get(), "PropertyEditor.AssetComboStyle")
			.ForegroundColor(FEditorStyle::GetColor("PropertyEditor.AssetName.ColorAndOpacity"))
			.OnGetMenuContent(this, &SLevelDesignerBuildingWidget::OnGetMenuContent)
			.ContentPadding(2.0f)
			.ButtonContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.Padding(1.0f)
				.AutoHeight()
				[
					AssetWidget
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
	];
}

void SLevelDesignerBuildingWidget::SetBuildingObject(UObject* NewAsset)
{
	if (UStaticMesh* NewStaticMesh = Cast<UStaticMesh>(NewAsset))
	{
		AssetThumbnail->SetAsset(NewAsset);
		LevelDesignerBuilding->SetBuildingObject(NewAsset);
	}
	else if (AActor* BlueprintAsset = Cast<AActor>(NewAsset))
	{
		AssetThumbnail->SetAsset(NewAsset);
		LevelDesignerBuilding->SetBuildingObject(NewAsset);
	}
}

bool SLevelDesignerBuildingWidget::IsAssetAcceptableForDrop(const UObject* InObject) const
{
	return (InObject->IsA(UStaticMesh::StaticClass()) || InObject->IsA(AActor::StaticClass()));
}

TSharedRef<SWidget> SLevelDesignerBuildingWidget::OnGetMenuContent()
{
	TArray<const UClass*> AllowedClasses;
	AllowedClasses.Add(UStaticMesh::StaticClass());
	AllowedClasses.Add(UBlueprint::StaticClass());


	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		NULL,
		false,
		AllowedClasses,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(AllowedClasses),
		FOnShouldFilterAsset::CreateRaw(this, &SLevelDesignerBuildingWidget::OnShouldFilterAsset),
		FOnAssetSelected::CreateSP(this, &SLevelDesignerBuildingWidget::OnAssetSelectedFromPicker),
		FSimpleDelegate::CreateSP(this, &SLevelDesignerBuildingWidget::CloseMenu));
}

void SLevelDesignerBuildingWidget::CloseMenu()
{
	ComboButton->SetIsOpen(false);
}

void SLevelDesignerBuildingWidget::OnAssetSelectedFromPicker(const FAssetData& InAssetData)
{
	SetBuildingObject(InAssetData.GetAsset());
}

void SLevelDesignerBuildingWidget::OnUseSelectedAsset()
{
	// Load selected assets
	FEditorDelegates::LoadSelectedAssetsIfNeeded.Broadcast();

	// Get the first static mesh selected
	USelection* AssetSelection = GEditor->GetSelectedObjects();
	if (AssetSelection && AssetSelection->Num() == 1)
	{
		UStaticMesh* StaticMeshToAssign = AssetSelection->GetTop<UStaticMesh>();
		if (StaticMeshToAssign)
			SetBuildingObject(StaticMeshToAssign);
		else
		{
			UBlueprint* ActorToAssign = AssetSelection->GetTop<UBlueprint>();
			if (ActorToAssign && ActorToAssign->ParentClass == AActor::StaticClass())
			{
				SetBuildingObject(ActorToAssign);
			}
		}
	}
}


bool SLevelDesignerBuildingWidget::CanUseSelectedAsset()
{
	bool Result = false;

	// Load selected assets
	FEditorDelegates::LoadSelectedAssetsIfNeeded.Broadcast();

	// Get the first static mesh selected
	USelection* AssetSelection = GEditor->GetSelectedObjects();
	if (AssetSelection && AssetSelection->Num() == 1)
	{
		UStaticMesh* StaticMeshToAssign = AssetSelection->GetTop<UStaticMesh>();
		if (StaticMeshToAssign)
		{
			Result = true;
		}
		else
		{
			UBlueprint* ActorToAssign = AssetSelection->GetTop<UBlueprint>();
			if (ActorToAssign && ActorToAssign->ParentClass == AActor::StaticClass())
			{
				Result = true;
			}
		}
	}

	return Result;
}


void SLevelDesignerBuildingWidget::OnBrowseToAsset()
{
	UObject* buildingObject = LevelDesignerBuilding->GetBuildingObject();

	// Find the item in the content browser
	if (buildingObject != NULL)
	{
		TArray< UObject* > Objects;
		Objects.Add(buildingObject);
		GEditor->SyncBrowserToObjects(Objects);
	}	
}


bool SLevelDesignerBuildingWidget::CanBrowseToAsset()
{
	const UObject* buildingObject = LevelDesignerBuilding->GetBuildingObject();

	return buildingObject != NULL;
}


void SLevelDesignerBuildingWidget::OnClearAsset()
{
	/* TODO*/
	UObject* buildingObject = LevelDesignerBuilding->GetBuildingObject();
	UObject* defaultBuildingObject = NULL;

	if (buildingObject != defaultBuildingObject)
	{
		SetBuildingObject(defaultBuildingObject);
	}
}


bool SLevelDesignerBuildingWidget::CanUseClearButton()
{
	const UObject* buildingObject = LevelDesignerBuilding->GetBuildingObject();
	const UObject* defaultBuildingObject = NULL;

	return buildingObject != defaultBuildingObject;
}

bool SLevelDesignerBuildingWidget::OnShouldFilterAsset(const FAssetData& AssetData) const
{
	const UObject* buildingObject = AssetData.GetAsset();

	if (buildingObject->IsA(UBlueprint::StaticClass()))
	{
		const UBlueprint* blueprintBuildingObject = static_cast<const UBlueprint*>(buildingObject);
		if (blueprintBuildingObject && blueprintBuildingObject->ParentClass != (AActor::StaticClass()))
		{
			return true;
		}
	}

	return false;
}