// Particle Rush 2014 - 2015

#pragma once

#include "ParticleRush.h"

namespace ParticleRush
{
	namespace Utilities
	{
		/*
		* Given a vector and a normal to the surface, GetReflectionVector computes and returns the normalized reflection vector
		*/
		FVector GetReflectionVector(FVector inVec, FVector normal)
		{
			return -2 * (FVector::DotProduct(inVec, normal)) * normal + inVec;
		}

		/*
		* Given two vectors, GetRotationBetweenVectors computes the euler angles required to rotate from Vector A to Vector B
		*/
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
	}
}