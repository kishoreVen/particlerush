// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "GrindActor.h"
#include "Components/SplineComponent.h"
#include "Generic/ParticleRushUtils.h"
#include "Character/RushCharacter.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AGrindActor::AGrindActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Component Setup
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetMobility(EComponentMobility::Static);

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->AttachParent = RootComponent;
	#pragma endregion

	mShouldActivateRamp = false;

	mPreviousClosestPoint = 0;

	mRushPtr = NULL;
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

	if (mShouldActivateRamp == false || mRushPtr == NULL)
		return;

	FVector refLoc = mRushPtr.Get()->GetActorLocation();
	int32 numSplinePoints = SplineComp->GetNumSplinePoints();
	int32 closestSplinePoint = UParticleRushUtils::GetClosestSplinePointIndex(SplineComp, refLoc, mPreviousClosestPoint, SplineSearchDistanceSqrdThreshold);

	/* Cant Find that point */
	if (closestSplinePoint == -1 || closestSplinePoint >= numSplinePoints)
		return;

	FVector closestSplinePointLocation, closestSplinePointTangent;
	SplineComp->GetLocalLocationAndTangentAtSplinePoint(closestSplinePoint, closestSplinePointLocation, closestSplinePointTangent);

	FVector nextSplinePointLocation, nextSplinePointTangent;
	SplineComp->GetLocalLocationAndTangentAtSplinePoint(closestSplinePoint + 1, nextSplinePointLocation, nextSplinePointTangent);

	FVector movementDirection = nextSplinePointLocation - closestSplinePointTangent;

	//mRushPtr.Get()->AddActorLocalOffset(movementDirection * DeltaTime, true);
	mRushPtr->SetActorLocation(FMath::VInterpConstantTo(mRushPtr->GetActorLocation(), closestSplinePointLocation, DeltaTime, 1.f));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, SplinePointLocation.ToString());

	mPreviousClosestPoint = closestSplinePoint;
}

void AGrindActor::NotifyActorBeginOverlap(class AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	/* Try to get it for the first time */
	ARushCharacter* rushActor = static_cast<ARushCharacter*>(OtherActor);
	if (rushActor == NULL)
		return;

	rushActor->SetInputDOFState(EInputDOF::EVERYTHING, false);
	mRushPtr = rushActor;
	mShouldActivateRamp = true;
}

void AGrindActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	/* Try to get it for the first time */
	ARushCharacter* rushActor = static_cast<ARushCharacter*>(OtherActor);
	if (rushActor == NULL)
		return;

	rushActor->ResetInputDOFStateToLastDisableMask();
	mRushPtr = NULL;

	mShouldActivateRamp = false;
	mPreviousClosestPoint = 0;
}
