#include "PrototypeLevelGeneratorPrivatePCH.h"

#include "PrototypeLevelGeneratorWindow.h"
#include "Generic/BaseMeshActor.h"
#include "Landscape.h"
#include "EditorActorFolders.h"

#define LOCTEXT_NAMESPACE "PrototypeLevelGeneratorWindow"


PrototypeLevelGeneratorWindow::PrototypeLevelGeneratorWindow()
:	mNumBuildingClasses(4),
	mAlleySpacingMin(10.0f),
	mAlleySpacingMax(100.0f),
	mMinBuildingDimensions(FVector(2, 2, 2)),
	mMaxBuildingDimensions(FVector(3, 3, 3))
{
	BuildingParamsWidget::DefaultMinDimension = mMinBuildingDimensions;
	BuildingParamsWidget::DefaultMaxDimension = mMaxBuildingDimensions;
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
				SNew(STextBlock).Text(LOCTEXT("AlleySpacing", "Alley Spacing (Min, Max)"))
			]

			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.Padding(FMargin(0, 0, 5, 0))
				[
					SNew(SNumericEntryBox<float>)
					.AllowSpin(true)
					.MinValue(1.0f)
					.MaxValue(10000.0f)
					.MinSliderValue(1.0f)
					.MaxSliderValue(10000.0f)
					.Value(this, &PrototypeLevelGeneratorWindow::GetAlleySpacingMin)
					.OnValueChanged(this, &PrototypeLevelGeneratorWindow::SetAlleySpacingMin)
				]

				+ SHorizontalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Center)
				.Padding(FMargin(5, 0, 0, 0))
				[
					SNew(SNumericEntryBox<float>)
					.AllowSpin(true)
					.MinValue(1.0f)
					.MaxValue(10000.0f)
					.MinSliderValue(1.0f)
					.MaxSliderValue(10000.0f)
					.Value(this, &PrototypeLevelGeneratorWindow::GetAlleySpacingMax)
					.OnValueChanged(this, &PrototypeLevelGeneratorWindow::SetAlleySpacingMax)
				]
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
			.Padding(10)
			[
				SAssignNew(BuildingMeshes[i], BuildingParamsWidget)
				.BuildingColor(colorList.GetFColorByIndex(colorIndex))
			];

		colorIndex += 10;
	}
}


void PrototypeLevelGeneratorWindow::UpdateBuildingParams(FVector NewMinDimension, FVector NewMaxDimension)
{
	for (int i = 0; i < mNumBuildingClasses; i++)
	{
		BuildingMeshes[i]->UpdateDefaultDimensions(NewMinDimension, NewMaxDimension);
	}

	BuildingParamsWidget::DefaultMinDimension = mMinBuildingDimensions;
	BuildingParamsWidget::DefaultMaxDimension = mMaxBuildingDimensions;
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


const AActor* PrototypeLevelGeneratorWindow::GenerateLevelObject(TSharedPtr<BuildingParamsWidget> buildingParams, FVector position, FName sceneOutlinerFolderName)
{
	static FName BaseColorParamName("BaseColor");

	UWorld* world = GetWorld();

	/* Extract Info */
	UStaticMesh* buildingMesh = buildingParams->BuildingMeshPtr.Get();
	FLinearColor linearBaseColor(buildingParams->BSPBuildingColor);

	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	ABaseMeshActor* spawnedBuilding = world->SpawnActor<ABaseMeshActor>(ABaseMeshActor::StaticClass(), position, FRotator::ZeroRotator, spawnParams);
	if (spawnedBuilding == NULL)
		return NULL;

	/* Move To Corresponding Folder */
	spawnedBuilding->SetFolderPath(sceneOutlinerFolderName);

	FVector minDimension = buildingParams->GetMinDimension();
	FVector maxDimension = buildingParams->GetMaxDimension();
	FVector dimension(FMath::FRandRange(minDimension.X, maxDimension.X), FMath::FRandRange(minDimension.Y, maxDimension.Y), FMath::FRandRange(minDimension.Z, maxDimension.Z));

	spawnedBuilding->SetStaticMesh(buildingMesh);
	spawnedBuilding->SetActorScale3D(minDimension);

	if (buildingParams->IsDefaultMesh())
	{
		UMaterialInstanceDynamic* materialInstance = spawnedBuilding->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, buildingMesh->GetMaterial(0));
		materialInstance->SetVectorParameterValue(BaseColorParamName, linearBaseColor);
	}

	return spawnedBuilding;
}


FReply PrototypeLevelGeneratorWindow::GeneratePrototypeLevelClicked()
{
	static FName GeneratedRootFolderName("Generated_Root");


	UWorld* world = GetWorld();

	TActorIterator<ALandscape> landscapeIterator(world);
	ALandscape* landscape = *landscapeIterator;

	FIntRect landscapeBounds = landscape->GetBoundingRect(); // Should return number of components
	FVector landscapeScale = landscape->GetActorScale3D(); // should return scale of each component
	FVector landscapeLocation = landscape->GetActorLocation(); // should return top left position of the landscape when viewed in top down
	FVector landscapeEndLocation(landscapeLocation);
	landscapeEndLocation.X += landscapeBounds.Max.X * landscapeScale.X;
	landscapeEndLocation.Y += landscapeBounds.Max.Y * landscapeScale.Y;


	/* Check if Head folder exists*/
	FActorFolders& SceneOutlinerFolder = FActorFolders::Get();
	FActorFolderProps* folderProps = SceneOutlinerFolder.GetFolderProperties(*world, GeneratedRootFolderName);
	if (folderProps == NULL)
	{
		SceneOutlinerFolder.DeleteFolder(*world, GeneratedRootFolderName);
	}

	/* Create Head Folder */
	SceneOutlinerFolder.CreateFolder(*world, GeneratedRootFolderName);

	for (float x = landscapeLocation.X; x < landscapeEndLocation.X;)
	{
		float maxWidth = 0;

		for (float y = landscapeLocation.Y; y < landscapeEndLocation.Y;)
		{
			int buildingType = FMath::RandRange(0, mNumBuildingClasses - 1);
			const AActor* spawnedActor = GenerateLevelObject(BuildingMeshes[buildingType], FVector(x, y, 0.0f), GeneratedRootFolderName);

			/* Update position */
			FVector origin, bounds;
			spawnedActor->GetActorBounds(true, origin, bounds);
			
			y += bounds.Y * 2 + FMath::FRandRange(mAlleySpacingMin, mAlleySpacingMax);

			if (bounds.X > maxWidth)
				maxWidth = bounds.X;
		}
		

		x += maxWidth * 2 + FMath::FRandRange(mAlleySpacingMin, mAlleySpacingMax);
	}

	return FReply::Handled();
}