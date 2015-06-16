// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "BarrierGenerator.h"

/* Engine Headers */
#include "Components/SplineComponent.h"


// Sets default values
ABarrierGenerator::ABarrierGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrierBase = CreateDefaultSubobject<USceneComponent>("BarrierBase");
	if (BarrierBase != NULL)
	{
		RootComponent = BarrierBase;
	}

	BarrierStart = CreateDefaultSubobject<UStaticMeshComponent>("BarrierStart");
	if (BarrierStart != NULL)
	{
		BarrierStart->AttachTo(BarrierBase, NAME_None, EAttachLocation::KeepRelativeOffset, true);
	}
}


// Called when the game starts or when spawned
void ABarrierGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABarrierGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MainBarrierMesh == NULL)
		return;

	/* Compute Mesh Params */
	FBox meshBoundingBox = MainBarrierMesh->GetBounds().GetBox();
	float meshWidth = meshBoundingBox.Max.X - meshBoundingBox.Min.X;

	/* Set the Mesh for Barrier Start */
	BarrierStart->SetStaticMesh(MainBarrierMesh);

	/* Rotate the Parent to face in current Direction */
	FVector flatDirectionToEndPoint = (EndPoint - BarrierStart->RelativeLocation) * FVector(1.0f, 1.0f, 0.0f);

	FRotator lookAtRotation(0.0f, flatDirectionToEndPoint.Rotation().Yaw, 0.0f);
	BarrierStart->SetWorldRotation(lookAtRotation);

	int32_t count = FMath::Round(flatDirectionToEndPoint.Size() / meshWidth);
	for (int index = 1; index <= count; index++)
	{
		FVector currentPosition = FVector(index * meshWidth, 0.0f, 0.0f);
		AddBarrierElement(MainBarrierMesh, currentPosition);
	}
}


void ABarrierGenerator::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}


#pragma region METHODS
void ABarrierGenerator::AddBarrierElement(UStaticMesh* staticMesh, const FVector& position)
{
	UStaticMeshComponent* tempMeshComponent = ConstructObject<UStaticMeshComponent>(UStaticMeshComponent::StaticClass(), this);
	tempMeshComponent->AttachTo(BarrierStart, NAME_None, EAttachLocation::KeepWorldPosition, true);
	tempMeshComponent->SetStaticMesh(staticMesh);
	tempMeshComponent->RelativeLocation = position;
}
#pragma endregion