// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleRush.h"

/**
 * Collective declerations and definitions of all math functions used
 */
namespace ParticleRush
{
	namespace Math
	{
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
