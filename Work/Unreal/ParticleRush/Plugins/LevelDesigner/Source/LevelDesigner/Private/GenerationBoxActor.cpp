#include "LevelDesignerPrivatePCH.h"

#include "GenerationBoxActor.h"
#include "Generic/BaseMeshActor.h"


AGenerationBoxActor::AGenerationBoxActor()
: MinAlleySpacing(10.0f)
, MaxAlleySpacing(100.0f)
, mCreationTimeMaxBounds(FVector::ZeroVector)
, mCreationTimeMinBounds(FVector::ZeroVector)
{
	RootSceneComponent = CreateAbstractDefaultSubobject<USceneComponent>("RootSceneComp");
	if (RootSceneComponent == NULL)
		return;

	RootComponent = RootSceneComponent;

	GenerationBox = CreateAbstractDefaultSubobject<UBoxComponent>("GenerationBox");
	if (GenerationBox == NULL)
		return;

	GenerationBox->AttachParent = RootSceneComponent;
}

AGenerationBoxActor::~AGenerationBoxActor()
{
	ClearGeneratedActors();
}


void AGenerationBoxActor::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	PopulateWorld();
}


void AGenerationBoxActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	PopulateWorld();
}


void AGenerationBoxActor::GetGenerationBounds(FVector& minBounds, FVector& maxBounds, bool zDesired)
{
	FVector halfBoxDimension = GenerationBox->Bounds.BoxExtent;
	if (!zDesired)
		halfBoxDimension.Z = 0.0f;

	FVector boxOrigin = GetActorLocation();
	
	minBounds = boxOrigin - halfBoxDimension;
	maxBounds = boxOrigin + halfBoxDimension;
}

void AGenerationBoxActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (!bFinished)
		return;

	FVector minBounds, maxBounds;
	GetGenerationBounds(minBounds, maxBounds);

	FVector movementDifference = minBounds - mCreationTimeMinBounds;
	mCreationTimeMinBounds = minBounds;
	mCreationTimeMaxBounds = maxBounds;

	int generatedActorsCount = GeneratedActors.Num();

	for (int index = 0; index < generatedActorsCount; index++)
	{
		FVector currentActorLocation = GeneratedActors[index]->GetActorLocation();
		GeneratedActors[index]->SetActorLocation(currentActorLocation + movementDifference);
	}

	PopulateWorld();
}


void AGenerationBoxActor::OnConstruction(const FTransform& Transform)
{
	FLevelDesignerModule& LevelDesignerModule = FModuleManager::Get().LoadModuleChecked<FLevelDesignerModule>("LevelDesigner");
	UObject* LevelDesignerAsset = LevelDesignerModule.GetLevelDesignerSettingsAsset();

	check(LevelDesignerAsset != NULL);
	
	LevelDesigner_SettingsAsset = static_cast<ULevelDesignerAsset*>(LevelDesignerAsset);

	PopulateWorld();
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

	GetGenerationBounds(mCreationTimeMinBounds, mCreationTimeMaxBounds);

	FVector CurrentActorLocation = mCreationTimeMinBounds;

	while (CurrentActorLocation.X < mCreationTimeMaxBounds.X)
	{
		float MaxWidth = 0.0f;

		CurrentActorLocation.Y = mCreationTimeMinBounds.Y;
		while (CurrentActorLocation.Y < mCreationTimeMaxBounds.Y)
		{
			int actorClassIndex = FMath::RandRange(0, LevelDesigner_SettingsAsset->LevelDesignerBuildings.Num() - 1);
			FLevelDesignerBuildingData actorClassData = LevelDesigner_SettingsAsset->LevelDesignerBuildings[actorClassIndex];

			FRotator actorRoation = FRotator::ZeroRotator;

			FVector actorDimensions;

			if (actorClassData.BuildingClass == UStaticMesh::StaticClass())
			{
				actorDimensions = SpawnActor(ABaseMeshActor::StaticClass(), CurrentActorLocation, actorRoation, actorClassData.BuildingColor, static_cast<UStaticMesh*>(actorClassData.BuildingObject));
			}
			else
			{
				actorDimensions = SpawnActor(actorClassData.BuildingClass, CurrentActorLocation, actorRoation, actorClassData.BuildingColor);
			}

			CurrentActorLocation.Y += actorDimensions.Y + FMath::FRandRange(MinAlleySpacing, MaxAlleySpacing);

			if (actorDimensions.X > MaxWidth)
				MaxWidth = actorDimensions.X;
		}

		CurrentActorLocation.X += MaxWidth + FMath::FRandRange(MinAlleySpacing, MaxAlleySpacing);
	}
}


void AGenerationBoxActor::MoveGeneratedActorsToNewLocation()
{

}


FVector AGenerationBoxActor::SpawnActor(UClass* ActorClass, const FVector& ActorLocation, const FRotator& ActorOrienation, const FColor& ActorClassColor, UStaticMesh* ActorMesh)
{
	static FName BaseColorParamName("BaseColor");
	UWorld* World = GetWorld();

	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	AActor* spawnedActor = World->SpawnActor<AActor>(ActorClass, ActorLocation, ActorOrienation, spawnParams);

	if (spawnedActor == NULL)
		return FVector::ZeroVector;

	if (spawnedActor->IsA(ABaseMeshActor::StaticClass()))
	{
		ABaseMeshActor* spawnedBuilding = static_cast<ABaseMeshActor*>(spawnedActor);

		spawnedBuilding->SetStaticMesh(ActorMesh);

		FVector minDimension = LevelDesigner_SettingsAsset->DefaultMinDimensions;
		FVector maxDimension = LevelDesigner_SettingsAsset->DefaultMaxDimensions;
		FVector dimension(FMath::FRandRange(minDimension.X, maxDimension.X), FMath::FRandRange(minDimension.Y, maxDimension.Y), FMath::FRandRange(minDimension.Z, maxDimension.Z));
		spawnedBuilding->SetActorScale3D(dimension);
		
		UMaterialInstanceDynamic* materialInstance = spawnedBuilding->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ActorMesh->GetMaterial(0));
		materialInstance->SetVectorParameterValue(BaseColorParamName, ActorClassColor);
	}

	spawnedActor->SetActorLocation(ActorLocation);
	spawnedActor->SetActorRotation(ActorOrienation);

	spawnedActor->Tags.Add(*ActorClassColor.ToHex());

	GeneratedActors.Add(spawnedActor);
	
	FVector origin, extent;
	spawnedActor->GetActorBounds(true, origin, extent);

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