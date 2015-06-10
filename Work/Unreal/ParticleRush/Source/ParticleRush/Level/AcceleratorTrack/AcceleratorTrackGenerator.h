// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "GameFramework/Actor.h"

/* Custom Headers */
#include "TrackData.h"

/* Generated Headers */
#include "AcceleratorTrackGenerator.generated.h"

UCLASS()
class PARTICLERUSH_API AAcceleratorTrackGenerator : public AActor
{
	GENERATED_BODY()

#pragma region CONSTRUCTOR
	// Sets default values for this actor's properties
	AAcceleratorTrackGenerator();
#pragma endregion

#pragma region COMPONENTS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Spline"))
	class USplineComponent* TrackSpline;
#pragma endregion

#pragma region OVERRIDES
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion

#pragma region PROPERTIES
protected:
	/* Specify this for telling how much control rush can have while on Track */
	UPROPERTY(EditAnywhere, Meta = (Category = "Control"))
	FRotator OnTrackRotationRotate;

	/* Specify this for telling how much control rush can have while on Track */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Track Build"))
	bool IsLoopingTrack;

	/* The data built from spline build */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Track Build"))
	TArray<FTrackData> TrackBuildData;
#pragma endregion
	
#pragma region PROPERTIY ACCESSORS
public:
	UFUNCTION(Meta = (Category = "Control"))
	FRotator GetOnTrackDeltaRotation(float DeltaTime);
#pragma endregion
	
#pragma region FIELDS
private:
#pragma endregion
};
