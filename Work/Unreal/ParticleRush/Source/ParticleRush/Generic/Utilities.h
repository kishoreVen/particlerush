// Particle Rush 2014 - 2015

#pragma once

namespace ParticleRush
{
	namespace Utilities
	{
		/*
		* Given two vectors, GetReflectionVector computes the reflection of inVec about the normal
		*/
		extern FVector GetReflectionVector(const FVector& inVec, const FVector& normal);

		/*
		* Given two vectors, GetRefractionVector computes the refraction of inVec about the normal, with refractive index. Give refractive index as nL / nR (0 ... 1)
		*/
		extern FVector GetRefractionVector(const FVector& inVec, const FVector& normal, float refractiveIndex);
		

		/*
		* Given two normalized vectors, GetRotationBetweenVectors computes the euler angles required to rotate from Vector A to Vector B
		*/
		extern FRotator GetRotationBetweenVectors(const FVector& vecA, const FVector& vecB);


		/*
		* The ROverShootingInterpTo will help creating an oscillating effect about the target rotation.
		*/
		extern FRotator ROverShootingInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);
	}
}