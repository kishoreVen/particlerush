// Particle Rush 2014 - 2015

#pragma once

#include "ParticleRush.h"
#include "Utilities.h"

namespace ParticleRush
{
	namespace Utilities
	{
		FVector GetReflectionVector(const FVector& inVec, const FVector& normal)
		{
			return -2 * (FVector::DotProduct(inVec, normal)) * normal + inVec;
		}


		FVector GetRefractionVector(const FVector& inVec, const FVector& normal, float refractiveIndex)
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

		
		FRotator GetRotationBetweenVectors(const FVector& vecA, const FVector& vecB)
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

		
		FRotator ROverShootingInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
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
	}
}