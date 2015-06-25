// Particle Rush 2014 - 2015

#pragma once

#include "ParticleRush.h"
#include "ParticleRushUtils.h"

#include "Components/SplineComponent.h"

FVector UParticleRushUtils::GetReflectionVector(const FVector& inVec, const FVector& normal)
{
	return -2 * (FVector::DotProduct(inVec, normal)) * normal + inVec;
}


FVector UParticleRushUtils::GetRefractionVector(const FVector& inVec, const FVector& normal, float refractiveIndex)
{
	if (refractiveIndex > 1 || refractiveIndex <= 0)
		return FVector::ZeroVector;

	float dotProduct = FVector::DotProduct(inVec, normal);
	float radical = FMath::Sqrt(1 - refractiveIndex * refractiveIndex * (1 - dotProduct * dotProduct));

	if (radical < 0)
		return FVector::ZeroVector;

	FVector normalContribution = (refractiveIndex * dotProduct - radical) * normal;
	FVector inVecContribution = refractiveIndex * inVec;

	FVector refractionVector = normalContribution - inVecContribution;
	refractionVector.Normalize();

	return refractionVector;
}


FRotator UParticleRushUtils::GetRotationBetweenVectors(const FVector& vecA, const FVector& vecB)
{
	FRotator angle;

	FVector cross = FVector::CrossProduct(vecA, vecB);
	cross.Normalize();

	float dot = FVector::DotProduct(vecA, vecB);
	float angleFromDot = FMath::Acos(dot);
	
	FQuat quat(cross, angleFromDot);

	angle = FRotator(quat);

	return angle;
}


FRotator UParticleRushUtils::ROverShootingInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
{
	// if DeltaTime is 0, do not perform any interpolation (Location was already calculated for that frame)
	if (DeltaTime == 0.f || Current == Target)
	{
		return Current;
	}

	// If no interp speed, jump to target value
	if (InterpSpeed <= 0.f)
	{
		return Target;
	}

	const float DeltaInterpSpeed = InterpSpeed * DeltaTime;

	const FRotator Delta = (Target - Current).GetNormalized();

	// If steps are too small, just return Target and assume we have reached our destination.
	if (Delta.IsNearlyZero())
	{
		return Target;
	}

	// Delta Move, Clamp so we do not over shoot.
	const FRotator DeltaMove = Delta * DeltaInterpSpeed;
	return (Current + DeltaMove).GetNormalized();
}

int32 UParticleRushUtils::GetClosestSplinePointIndex(const USplineComponent* SplineComponent, const FVector& ReferenceLocation, int32 SearchStartAt, float DistanceThresholdSqrd)
{
	int32 numSplinePoints = SplineComponent->GetNumSplinePoints();

	int32 closestSplinePointIndex = -1;
	float minDistance = DistanceThresholdSqrd;

	/* Start from the index specified at Search Start At */
	for (int32 splineIndex = SearchStartAt; splineIndex < numSplinePoints; splineIndex++)
	{
		FVector location, tangent;
		SplineComponent->GetLocalLocationAndTangentAtSplinePoint(splineIndex, location, tangent);

		float distance = (location - ReferenceLocation).SizeSquared();
		if (distance < minDistance)
			closestSplinePointIndex = splineIndex;
	}

	if (-1 == closestSplinePointIndex)
	{
		for (int32 splineIndex = 0; splineIndex < SearchStartAt; splineIndex++)
		{
			FVector location, tangent;
			SplineComponent->GetLocalLocationAndTangentAtSplinePoint(splineIndex, location, tangent);

			float distance = (location - ReferenceLocation).SizeSquared();
			if (distance < minDistance)
				closestSplinePointIndex = splineIndex;
		}
	}

	return closestSplinePointIndex;
}