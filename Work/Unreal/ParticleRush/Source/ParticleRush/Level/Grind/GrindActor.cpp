// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "GrindActor.h"
#include "Components/SplineComponent.h"
#include "Generic/ParticleRushUtils.h"

// Sets default values
AGrindActor::AGrindActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	#pragma region Component Setup
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetMobility(EComponentMobility::Static);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->AttachParent = RootComponent;
	#pragma endregion

}

// Called when the game starts or when spawned
void AGrindActor::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AGrindActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGrindActor::NotifyActorBeginOverlap(class AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	FVector RefLoc = OtherActor->GetActorLocation();
	int32 BeginSplineSearch = 0;
	float DistThreshSquared = 100.0f;
	int32 NumSplinePoints = SplineComp->GetNumSplinePoints();
	int32 ClosestSplinePoint = UParticleRushUtils::GetClosestSplinePointIndex(SplineComp, RefLoc, BeginSplineSearch, DistThreshSquared);

	for (ClosestSplinePoint = BeginSplineSearch; ClosestSplinePoint < NumSplinePoints; ClosestSplinePoint++)
	{
		FVector SplinePointLocation, SplinePointTangent;
		SplineComp->GetLocalLocationAndTangentAtSplinePoint(ClosestSplinePoint, SplinePointLocation, SplinePointTangent);
		
		OtherActor->SetActorLocation(SplinePointLocation);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, SplinePointLocation.ToString());
	}
}
