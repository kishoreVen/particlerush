// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "TLaser.h"
#include "Generic/ParticleRushUtils.h"
#include "Math/UnrealMathUtility.h"
#include "Character/RushCharacter.h"


// Sets default values
ATLaser::ATLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Component Setup
	StaticMeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBase"));
	RootComponent = StaticMeshBase;
	StaticMeshBase->SetMobility(EComponentMobility::Movable);

	StaticMeshNozzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshNozzle"));
	StaticMeshNozzle->AttachParent = RootComponent;
#pragma endregion
}

// Called when the game starts or when spawned
void ATLaser::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATLaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (mRushPtr == NULL)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Nice Meme :^)");
		return;

	FVector refLoc = mRushPtr.Get()->GetActorLocation();
	FVector originLoc = this->GetActorLocation();

	FVector finalLoS = refLoc - originLoc;
	FVector finalMovement = finalLoS.GetSafeNormal();

	FRotator currentActorRotation = this->GetActorRotation();
	FRotator finalActorRotation = finalMovement.Rotation();
	FRotator interpedRotation = FMath::RInterpTo(currentActorRotation, finalActorRotation, DeltaTime, 0.7f);
	this->SetActorRotation(interpedRotation);
}

