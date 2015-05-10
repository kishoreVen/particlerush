// Particle Rush 2014 - 2015

#pragma once

namespace ParticleRush
{
	namespace Utilities
	{
		/*
		* Given two vectors, GetReflectionVector computes the reflection of inVec about the normal
		*/
		extern FVector GetReflectionVector(FVector inVec, FVector normal);
		

		/*
		* Given two vectors, GetRotationBetweenVectors computes the euler angles required to rotate from Vector A to Vector B
		*/
		extern FRotator GetRotationBetweenVectors(FVector vecA, FVector vecB);


		/*
		* The ROverShootingInterpTo will help creating an oscillating effect about the target rotation.
		*/
		FRotator ROverShootingInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);
	}
}