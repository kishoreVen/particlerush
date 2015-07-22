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
	int32 closestSplinePoint = UParticleRushUtils::GetClosestSplinePointIndex(SplineComp, refLoc, SplineSearchDistanceSqrdThreshold);

	/* Cant Find that point */
	if (closestSplinePoint == -1 || closestSplinePoint >= numSplinePoints - 1)
		return;

	FVector closestSplinePointLocation	= SplineComp->GetWorldLocationAtSplinePoint(closestSplinePoint);
	FVector nextSplinePointLocation		= SplineComp->GetWorldLocationAtSplinePoint(closestSplinePoint + 1);

	/* Direction from one spline point to the next. This gives the direction we are supposed to move along the spline */
	FVector directionOfMovementAlongSpline	= nextSplinePointLocation - closestSplinePointLocation;
	directionOfMovementAlongSpline.Z		= 0;
	directionOfMovementAlongSpline			= directionOfMovementAlongSpline.GetSafeNormal();

	/* Direction from rush to the next spline point. This will give the gap we are supposed bridge as we move along the spline */
	FVector directionToNextClosestPoint		= nextSplinePointLocation - refLoc;
	directionToNextClosestPoint.Z			= 0;
	directionToNextClosestPoint				= directionToNextClosestPoint.GetSafeNormal();

	FVector netMovementDirection = directionToNextClosestPoint /*+ directionOfMovementAlongSpline*/;
	netMovementDirection = netMovementDirection.GetSafeNormal();

	FRotator currentActorRotation = mRushPtr.Get()->GetControllerRotation();
	FRotator finalActorRotation = netMovementDirection.Rotation();

	FRotator interpedRotation = FMath::RInterpTo(currentActorRotation, finalActorRotation, DeltaTime, 0.7f);

	if (GEngine)
	{
		DrawDebugDirectionalArrow(GetWorld(), refLoc, refLoc + netMovementDirection * 50.0f, 5.0f, FColor::Yellow);
		DrawDebugSphere(GetWorld(), refLoc, 10.0f, 16, FColor::Green);
		DrawDebugSphere(GetWorld(), closestSplinePointLocation, 10.0f, 16, FColor::Red);
		DrawDebugSphere(GetWorld(), nextSplinePointLocation, 10.0f, 16, FColor::Blue);
	}

	mRushPtr.Get()->SetControllerRotation(interpedRotation);
	mRushPtr.Get()->AddMovementInput(mRushPtr.Get()->GetActorForwardVector(), 0.1f);
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
}