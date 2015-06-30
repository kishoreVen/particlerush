#include "PrototypeLevelGeneratorPrivatePCH.h"

#include "PrototypeLevelGeneratorWindow.h"
#include "Generic/BaseMeshActor.h"

#define LOCTEXT_NAMESPACE "PrototypeLevelGeneratorWindow"


PrototypeLevelGeneratorWindow::PrototypeLevelGeneratorWindow()
:	mMapSizeX(64),
	mMapSizeY(64),
	mNumBuildingClasses(4),
	mAlleySpacing(10.0f),
	mMinBuildingDimensions(FVector(5, 5, 5)),
	mMaxBuildingDimensions(FVector(10, 10, 10))
{

}


void PrototypeLevelGeneratorWindow::Construct(const FArguments& args)
{
	TSharedRef<SVerticalBox> verticalBox = SNew(SVerticalBox);
	BuildingClassesVerticalSlot = SNew(SVerticalBox);

	/* A one time update when the window loads for the first time. */
	UpdateBuildingClassesSlot();

	ChildSlot
	[
		SNew(SScrollBox)
		.Orientation(Orient_Vertical)
		.ScrollBarAlwaysVisible(true)
		+ SScrollBox::Slot()
		[
			verticalBox
		]
	];

	verticalBox->AddSlot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("MapSize", "Map Size"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.Padding(FMargin(0, 0, 5, 0))
				[
					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinValue(64)
					.MaxValue(8192)
					.MinSliderValue(64)
					.MaxSliderValue(8192)
					.Value(this, &PrototypeLevelGeneratorWindow::GetMapSizeX)
					.OnValueChanged(this, &PrototypeLevelGeneratorWindow::SetMapSizeX)
				]

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.Padding(FMargin(5, 0, 0, 0))
				[
					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinValue(64)
					.MaxValue(8192)
					.MinSliderValue(64)
					.MaxSliderValue(8192)
					.Value(this, &PrototypeLevelGeneratorWindow::GetMapSizeY)
					.OnValueChanged(this, &PrototypeLevelGeneratorWindow::SetMapSizeY)
				]
			]
		];

	verticalBox->AddSlot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("AlleySpacing", "Alley Spacing"))
			]

			+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SNumericEntryBox<float>)
					.AllowSpin(true)
					.MinValue(0.0f)
					.MaxValue(50000.0f)
					.MinSliderValue(0.0f)
					.MaxSliderValue(50000.0f)
					.Value(this, &PrototypeLevelGeneratorWindow::GetAlleySpacing)
					.OnValueChanged(this, &PrototypeLevelGeneratorWindow::SetAlleySpacing)
				]
		];

	verticalBox->AddSlot()
		.AutoHeight()
		.Padding(FMargin(5, 5, 5, 1))
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("MinBuildingDimensions", "Minimum Dimensions for Buildings"))
			]

			+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SVectorInputBox)
					.bColorAxisLabels(true)
					.X(this, &PrototypeLevelGeneratorWindow::GetMinBuildingDimensionsX)
					.Y(this, &PrototypeLevelGeneratorWindow::GetMinBuildingDimensionsY)
					.Z(this, &PrototypeLevelGeneratorWindow::GetMinBuildingDimensionsZ)
					.OnXCommitted(this, &PrototypeLevelGeneratorWindow::SetMinBuildingDimensionsX)
					.OnYCommitted(this, &PrototypeLevelGeneratorWindow::SetMinBuildingDimensionsY)
					.OnZCommitted(this, &PrototypeLevelGeneratorWindow::SetMinBuildingDimensionsZ)
				]
		];

	verticalBox->AddSlot()
		.AutoHeight()
		.Padding(FMargin(5, 1, 5, 5))
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("MaxBuildingDimensions", "Maximum Dimensions for Buildings"))
			]

			+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SVectorInputBox)
					.bColorAxisLabels(true)
					.X(this, &PrototypeLevelGeneratorWindow::GetMaxBuildingDimensionsX)
					.Y(this, &PrototypeLevelGeneratorWindow::GetMaxBuildingDimensionsY)
					.Z(this, &PrototypeLevelGeneratorWindow::GetMaxBuildingDimensionsZ)
					.OnXCommitted(this, &PrototypeLevelGeneratorWindow::SetMaxBuildingDimensionsX)
					.OnYCommitted(this, &PrototypeLevelGeneratorWindow::SetMaxBuildingDimensionsY)
					.OnZCommitted(this, &PrototypeLevelGeneratorWindow::SetMaxBuildingDimensionsZ)
				]
		];

	verticalBox->AddSlot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock).Text(LOCTEXT("NumBuildingClasses", "Number of Building Classes"))
			]

			+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinValue(1)
					.MaxValue(50)
					.MinSliderValue(1)
					.MaxSliderValue(50)
					.Value(this, &PrototypeLevelGeneratorWindow::GetNumBuildingClasses)
					.OnValueChanged(this, &PrototypeLevelGeneratorWindow::SetNumBuildingClasses)
				]
		];

	verticalBox->AddSlot()
		.AutoHeight()
		.Padding(FMargin(50, 2.5, 5, 0))
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				BuildingClassesVerticalSlot.ToSharedRef()
			]
		];

	verticalBox->AddSlot()
	.AutoHeight()
	.Padding(5)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Default")
			.ForegroundColor(FLinearColor::White)
			.ButtonColorAndOpacity(FColorList::Grey)
			.VAlign(EVerticalAlignment::VAlign_Center)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.Text(LOCTEXT("GeneratePrototypeLevel", "Generate Level"))
			.OnClicked(this, &PrototypeLevelGeneratorWindow::GeneratePrototypeLevelClicked)
		]
	];
}

void PrototypeLevelGeneratorWindow::UpdateBuildingClassesSlot()
{
	BuildingClassesVerticalSlot->ClearChildren();

	int existingMeshCount = BuildingMeshes.Num();

	int colorIndex = 0;
	static FColorList colorList;
	colorList.CreateColorMap();

	if (mNumBuildingClasses > existingMeshCount)
	{
		for (int i = 0; i < (mNumBuildingClasses - existingMeshCount); i++)
		{
			TSharedPtr<BuildingParamsWidget> temp;
			BuildingMeshes.Add(temp);
		}
	}
	else
	{
		for (int i = 0; i < (existingMeshCount - mNumBuildingClasses); i++)
		{
			BuildingMeshes.Pop();
		}
	}

	for (int i = 0; i < mNumBuildingClasses; i++)
	{
		BuildingClassesVerticalSlot->AddSlot()
			.AutoHeight()
			.Padding(3)
			[
				SAssignNew(BuildingMeshes[i], BuildingParamsWidget)
				.BuildingColor(colorList.GetFColorByIndex(colorIndex))
			];

		colorIndex += 10;
	}
}


UWorld* PrototypeLevelGeneratorWindow::GetWorld()
{
	UWorld* world = NULL;
	const TIndirectArray<FWorldContext>& contexts = GEngine->GetWorldContexts();

	for (int32 i = 0; i < contexts.Num(); i++)
	{
		if (contexts[i].WorldType == EWorldType::PIE)
			return contexts[i].World();

		if (contexts[i].WorldType == EWorldType::Editor)
			world = contexts[i].World();
	}

	return world;
}


void PrototypeLevelGeneratorWindow::GenerateLevelObject(TSharedPtr<BuildingParamsWidget> buildingParams, FVector position, FVector dimension)
{
	static FName BaseColorParamName("BaseColor");

	UWorld* world = GetWorld();

	/* Extract Info */
	UStaticMesh* buildingMesh = buildingParams->BuildingMeshPtr.Get();
	FLinearColor linearBaseColor(buildingParams->BSPBuildingColor);

	ABaseMeshActor* spawnedBuilding = world->SpawnActor<ABaseMeshActor>(ABaseMeshActor::StaticClass());
	spawnedBuilding->SetStaticMesh(buildingMesh);

	if (buildingParams->IsDefaultMesh())
	{
		UMaterialInstanceDynamic* materialInstance = spawnedBuilding->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, buildingMesh->GetMaterial(0));
		materialInstance->SetVectorParameterValue(BaseColorParamName, linearBaseColor);
	}
}


FReply PrototypeLevelGeneratorWindow::GeneratePrototypeLevelClicked()
{
	UWorld* world = GetWorld();

	GenerateLevelObject(BuildingMeshes[0], FVector(0, 0, 0), FVector(1, 1, 1));
	GenerateLevelObject(BuildingMeshes[1], FVector(100, 0, 0), FVector(1, 1.5, 2));

	return FReply::Handled();
}