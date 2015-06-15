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

	BarrierSpline = CreateDefaultSubobject<USplineComponent>("BarrierSpline");
	if (BarrierSpline != NULL)
	{
		RootComponent = BarrierSpline;
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

}


void ABarrierGenerator::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}


#pragma region METHODS
void ABarrierGenerator::AddBarrierElement(UStaticMesh* staticMesh, const int32_t currentIndex)
{
	int32_t futureIndex = (currentIndex + 1) % BarrierSpline->GetNumSplinePoints();

	FBox meshBounds = staticMesh->GetBounds().GetBox();
	float meshWidth = meshBounds.Max.X - meshBounds.Min.X;
}
#pragma endregion