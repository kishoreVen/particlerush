// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "AcceleratorTrackGenerator.h"

/* Engine Headers */
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AAcceleratorTrackGenerator::AAcceleratorTrackGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TrackSpline = CreateDefaultSubobject<USplineComponent>("TrackSpline");
	if (TrackSpline != NULL)
	{
		RootComponent = TrackSpline;
	}
}

// Called when the game starts or when spawned
void AAcceleratorTrackGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAcceleratorTrackGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	int32_t numSplinePoints = TrackSpline->GetNumSplinePoints();
	int32_t trackEndIndex	= (IsLoopingTrack ? numSplinePoints - 1 : numSplinePoints - 2);
	int32_t numTrackPoints	= TrackBuildData.Num();

	/* Synchronize Spline points and Track Build Data */
	if (numSplinePoints < numTrackPoints) 
	{
		/* Locate mismatches using the stored location point and the points in spline */
		for (int32_t i = 0; i < numSplinePoints; i++)
		{
			if (!(TrackSpline->GetWorldLocationAtSplinePoint(i) - TrackBuildData[i].SplinePointLocation).IsNearlyZero())
			{
				TrackBuildData.RemoveAt(i);
			}
		}
		numTrackPoints = TrackBuildData.Num();

		/* Remove the rest from the end */
		for (int32_t i = numSplinePoints; i < numTrackPoints; i++)
		{
			TrackBuildData.Pop();
		}
		numTrackPoints = TrackBuildData.Num();
	}
	else if (numSplinePoints > numTrackPoints)
	{
		for (int32_t i = 0; i <= (numSplinePoints - numTrackPoints); i++)
		{
			/* Create a new track data at the end of current array */
			FTrackData newTrackData				= DefaultTrackData;
			newTrackData.SplinePointLocation	= TrackSpline->GetWorldLocationAtSplinePoint(numTrackPoints + i);

			/* Add element to array */
			TrackBuildData.Add(newTrackData);
		}

		/* Cache update for array size */
		numTrackPoints = TrackBuildData.Num();
	}

	/* Build the Mesh */
	for (int32_t trackIndex = 0; trackIndex <= trackEndIndex; trackIndex++)
	{
		AddTrackElement(MainTrackMesh, trackIndex);

		if (TrackBuildData[trackIndex].IsTunnel)
			AddTrackElement(TunnelTrackMesh, trackIndex);
	}
}


void AAcceleratorTrackGenerator::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}


#pragma region PROPERTIY ACCESSORS
FRotator AAcceleratorTrackGenerator::GetOnTrackDeltaRotation(float DeltaTime)
{
	return OnTrackRotationRotate * DeltaTime;
}
#pragma endregion


#pragma region METHODS
void AAcceleratorTrackGenerator::AddTrackElement(UStaticMesh* staticMesh, const int32_t currentIndex)
{
	int32_t futureIndex		= (currentIndex + 1) % TrackSpline->GetNumSplinePoints();

	FTrackData currentData	= TrackBuildData[currentIndex];
	FTrackData futureData	= TrackBuildData[futureIndex];

	FVector startPosition, endPosition, startTangent, endTangent;

	TrackSpline->GetLocalLocationAndTangentAtSplinePoint(currentIndex, startPosition, startTangent);
	TrackSpline->GetLocalLocationAndTangentAtSplinePoint(futureIndex, endPosition, endTangent);

	USplineMeshComponent* tempSplineMeshComp = ConstructObject<USplineMeshComponent>(USplineMeshComponent::StaticClass(), this);
	tempSplineMeshComp->AttachParent = TrackSpline;
	tempSplineMeshComp->SetStaticMesh(staticMesh);
	tempSplineMeshComp->SetStartAndEnd(startPosition, startTangent, endPosition, endTangent);
	tempSplineMeshComp->SetStartScale(currentData.TrackScale);
	tempSplineMeshComp->SetEndScale(futureData.TrackScale);
	tempSplineMeshComp->SetStartRoll(currentData.BankingAngle);
	tempSplineMeshComp->SetEndRoll(futureData.BankingAngle);

	IsCollisionOnTrackEnabled ? tempSplineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics) : tempSplineMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
#pragma endregion