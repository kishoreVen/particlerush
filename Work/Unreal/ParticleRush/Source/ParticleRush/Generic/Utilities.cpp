// Particle Rush 2014 - 2015

#pragma once

#include "ParticleRush.h"
#include "Utilities.h"

namespace ParticleRush
{
	namespace Utilities
	{
		FVector GetReflectionVector(FVector inVec, FVector normal)
		{
			return -2 * (FVector::DotProduct(inVec, normal)) * normal + inVec;
		}

		
		FRotator GetRotationBetweenVectors(FVector vecA, FVector vecB)
		{
			FRotator angle;

			vecA.Normalize();
			vecB.Normalize();

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