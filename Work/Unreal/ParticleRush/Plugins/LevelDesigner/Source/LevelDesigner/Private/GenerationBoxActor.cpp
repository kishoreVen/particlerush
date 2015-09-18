#include "LevelDesignerPrivatePCH.h"

#include "GenerationBoxActor.h"
#include "Generic/BaseMeshActor.h"


AGenerationBoxActor::AGenerationBoxActor()
: MinAlleySpacing(10.0f)
, MaxAlleySpacing(100.0f)
, DefaultMinAlleySpacing(true)
, DefaultMaxAlleySpacing(true)
{
	GenerationBox = CreateAbstractDefaultSubobject<UBoxComponent>("GenerationBox");
	if (GenerationBox == NULL)
		return;

	RootComponent = GenerationBox;
}

AGenerationBoxActor::~AGenerationBoxActor()
{
}


bool AGenerationBoxActor::RemoveSpawnedActor(AActor* aAcorToRemove)
{
	int numRemoved = GeneratedActors.Remove(aAcorToRemove);

	return numRemoved > 0;
}


void AGenerationBoxActor::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == "MinAlleySpacing")
	{
		if (MinAlleySpacing != LevelDesigner_SettingsAsset->DefaultMinAlleySpacing)
		{
			DefaultMinAlleySpacing = false;
		}

		if (MinAlleySpacing == 0.0f || MinAlleySpacing == LevelDesigner_SettingsAsset->DefaultMinAlleySpacing)
		{
			MinAlleySpacing = LevelDesigner_SettingsAsset->DefaultMinAlleySpacing;
			DefaultMinAlleySpacing = true;
		}

		PopulateWorld();
	}
	else if (PropertyName == "MaxAlleySpacing")
	{
		if (MaxAlleySpacing != LevelDesigner_SettingsAsset->DefaultMaxAlleySpacing)
		{
			DefaultMaxAlleySpacing = false;
		}

		if (MaxAlleySpacing == 0.0f || MaxAlleySpacing == LevelDesigner_SettingsAsset->DefaultMaxAlleySpacing)
		{
			MaxAlleySpacing = LevelDesigner_SettingsAsset->DefaultMaxAlleySpacing;
			DefaultMaxAlleySpacing = true;
		}

		PopulateWorld();
	}
}


void AGenerationBoxActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!PropertyChangedEvent.Property)
		return;

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	/* Make a Change Here */
	if (PropertyName == "RelativeLocation")
	{
		FCreationData postEditCreationData;
		GetGenerationBounds(postEditCreationData);

		FVector movementDifference = postEditCreationData.MinBoundsValue - CreationTimeData.MinBoundsValue;
		MoveGeneratedActorsToNewLocation(movementDifference);

		CreationTimeData = postEditCreationData;
	}
	else if (PropertyName == "RelativeScale3D")
	{
		PopulateWorld();
	}
	else if (PropertyName == "RelativeRotation")
	{
		SetActorRotation(FRotator::ZeroRotator);
		GenerationBox->SetRelativeRotation(FRotator::ZeroRotator);
	}
}


void AGenerationBoxActor::GetGenerationBounds(FCreationData& creationData)
{
	creationData.BoxExtent = GenerationBox->Bounds.BoxExtent;
	
	FVector boxOrigin = GetActorLocation();
	
	creationData.MinBoundsValue = boxOrigin - creationData.BoxExtent;
	creationData.MaxBoundsValue = boxOrigin + creationData.BoxExtent;
}

void AGenerationBoxActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	SetActorRotation(FRotator::ZeroRotator);
	GenerationBox->SetRelativeRotation(FRotator::ZeroRotator);

	FCreationData postEditCreationData;
	GetGenerationBounds(postEditCreationData);

	if (CreationTimeData.BoxExtent != postEditCreationData.BoxExtent)
	{
		PopulateWorld();
	}
	else
	{
		FVector movementDifference = postEditCreationData.MinBoundsValue - CreationTimeData.MinBoundsValue;
		MoveGeneratedActorsToNewLocation(movementDifference);

		CreationTimeData = postEditCreationData;
	}
}


void AGenerationBoxActor::Destroyed()
{
	Super::Destroyed();

	ClearGeneratedActors();
}


void AGenerationBoxActor::OnConstruction(const FTransform& Transform)
{
	FLevelDesignerModule& LevelDesignerModule = FModuleManager::Get().LoadModuleChecked<FLevelDesignerModule>("LevelDesigner");
	UObject* LevelDesignerAsset = LevelDesignerModule.GetLevelDesignerSettingsAsset();

	check(LevelDesignerAsset != NULL);
	
	LevelDesigner_SettingsAsset = static_cast<ULevelDesignerAsset*>(LevelDesignerAsset);

	if (!IsPopulatedBefore)
	{
		MinAlleySpacing = LevelDesigner_SettingsAsset->DefaultMinAlleySpacing;
		MaxAlleySpacing = LevelDesigner_SettingsAsset->DefaultMaxAlleySpacing;

		PopulateWorld();
		IsPopulatedBefore = true;
	}
	else
	{
		GetGenerationBounds(CreationTimeData);
	}
}


UWorld* GetWorld()
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


void AGenerationBoxActor::PopulateWorld()
{
	ClearGeneratedActors();
	
	GetGenerationBounds(CreationTimeData);
	
	FVector CurrentActorLocation = CreationTimeData.MinBoundsValue;
	while (CurrentActorLocation.X < CreationTimeData.MaxBoundsValue.X)
	{
		float MaxWidth = 0.0f;
		CurrentActorLocation.Y = CreationTimeData.MinBoundsValue.Y;
		while (CurrentActorLocation.Y < CreationTimeData.MaxBoundsValue.Y)
		{
			int actorClassIndex = FMath::RandRange(0, LevelDesigner_SettingsAsset->LevelDesignerBuildings.Num() - 1);
			FLevelDesignerBuildingData actorClassData = LevelDesigner_SettingsAsset->LevelDesignerBuildings[actorClassIndex];

			/* To Do .. Rotation Variance */
			FRotator actorRoation = FRotator::ZeroRotator;

			FVector actorDimensions;
			if (actorClassData.BuildingBlueprint->GeneratedClass->IsChildOf(ABaseMeshActor::StaticClass()))
			{
				actorDimensions = SpawnActor(actorClassData, CurrentActorLocation, actorRoation);
			}

			CurrentActorLocation.Y += actorDimensions.Y + FMath::FRandRange(MinAlleySpacing, MaxAlleySpacing);

			if (actorDimensions.X > MaxWidth)
				MaxWidth = actorDimensions.X;
		}

		CurrentActorLocation.X += MaxWidth + FMath::FRandRange(MinAlleySpacing, MaxAlleySpacing);
	}
}


void AGenerationBoxActor::MoveGeneratedActorsToNewLocation(FVector movementOffset)
{
	int generatedActorsCount = GeneratedActors.Num();
	for (int index = 0; index < generatedActorsCount; index++)
	{
		FVector currentActorLocation = GeneratedActors[index]->GetActorLocation();
		GeneratedActors[index]->SetActorLocation(currentActorLocation + movementOffset);
	}
}


FVector AGenerationBoxActor::SpawnActor(const FLevelDesignerBuildingData& LevelDesignerBuildingData, const FVector& ActorLocation, const FRotator& ActorOrienation)
{
	static FName BaseColorParamName("BaseColor");
	UWorld* World = GetWorld();

	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	UClass* ActorClass = LevelDesignerBuildingData.BuildingBlueprint->GeneratedClass;
	AActor* spawnedActor = World->SpawnActor<AActor>(ActorClass, ActorLocation, ActorOrienation, spawnParams);
	if (spawnedActor == NULL)
		return FVector::ZeroVector;

	GeneratedActors.Add(spawnedActor);

	ABaseMeshActor* spawnedBuilding = static_cast<ABaseMeshActor*>(spawnedActor);

	if (spawnedBuilding->GetStaticMesh() == NULL)
		spawnedBuilding->SetStaticMesh(LevelDesignerBuildingData.DefaultBuildingMesh);

	FVector minDimension = LevelDesignerBuildingData.MinDimensions;
	FVector maxDimension = LevelDesignerBuildingData.MaxDimensions;
	FVector dimension(FMath::FRandRange(minDimension.X, maxDimension.X), FMath::FRandRange(minDimension.Y, maxDimension.Y), FMath::FRandRange(minDimension.Z, maxDimension.Z));
	spawnedBuilding->SetActorScale3D(dimension);
		
	UMaterialInstanceDynamic* materialInstance = spawnedBuilding->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, LevelDesignerBuildingData.DefaultBuildingMesh->GetMaterial(0));
	materialInstance->SetVectorParameterValue(BaseColorParamName, LevelDesignerBuildingData.BuildingColor);

	spawnedBuilding->SetActorLocation(ActorLocation);
	spawnedBuilding->SetActorRotation(ActorOrienation);

	spawnedBuilding->Tags.Add(*(LevelDesignerBuildingData.BuildingColor.ToHex()));

	FVector origin, extent;
	spawnedBuilding->GetActorBounds(true, origin, extent);

	extent *= 2.0f; //Twice the actual size of the box, because extent is only half size

	return extent;
}


void AGenerationBoxActor::ClearGeneratedActors()
{
	int generatedActorsCount = GeneratedActors.Num();

	for (int index = 0; index < generatedActorsCount; index++)
	{
		GeneratedActors[index]->Destroy();
	}

	GeneratedActors.Empty();
}


void AGenerationBoxActor::ClearGeneratedActorsWithColor(const FColor& ActorClassColor)
{
	int generatedActorsCount = GeneratedActors.Num();
	TArray<int> RemovedIndices;

	for (int index = 0; index < generatedActorsCount; index++)
	{
		if (GeneratedActors[index]->ActorHasTag(*(ActorClassColor.ToHex())))
		{
			GeneratedActors[index]->Destroy();

			RemovedIndices.Add(index);
		}
	}

	for (int removedIndex = 0; removedIndex < RemovedIndices.Num(); removedIndex++)
	{
		GeneratedActors.RemoveAt(RemovedIndices[removedIndex]);
	}
}


void AGenerationBoxActor::RequestPopulateWorld()
{
	PopulateWorld();
}