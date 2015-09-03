// Particle Rush 2014 - 2015

#pragma once

#include "ParticleRushUtils.generated.h"

UCLASS()
class UParticleRushUtils : public UObject
{
	GENERATED_BODY()

public:
	/*
	* Given two vectors, GetReflectionVector computes the reflection of inVec about the normal
	*/
	static FVector GetReflectionVector(const FVector& inVec, const FVector& normal);

	/*
	* Given two vectors, GetRefractionVector computes the refraction of inVec about the normal, with refractive index. Give refractive index as nL / nR (0 ... 1)
	*/
	static FVector GetRefractionVector(const FVector& inVec, const FVector& normal, float refractiveIndex);
	

	/*
	* Given two normalized vectors, GetRotationBetweenVectors computes the euler angles required to rotate from Vector A to Vector B
	*/
	static FRotator GetRotationBetweenVectors(const FVector& vecA, const FVector& vecB);


	/*
	* The ROverShootingInterpTo will help creating an oscillating effect about the target rotation.
	*/
	static FRotator ROverShootingInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);
	
	/* 
	* Gets the closest spline point along the spline point index. Returns -1 if threshold distance condition is not met.
	*/
	UFUNCTION(BlueprintCallable, Meta = (Category = "Spline Utilities"))
	static int32 GetClosestSplinePointIndex(const class USplineComponent* SplineComponent, const FVector& ReferenceLocation, float DistanceThresholdSqrd = 1.0E06f);
};