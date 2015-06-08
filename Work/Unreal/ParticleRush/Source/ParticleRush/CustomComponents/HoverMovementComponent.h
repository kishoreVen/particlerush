// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "HoverMovementComponent.generated.h"

/**
 * Custom movement component for object that hover, i.e fly and react to gravity
 */
UCLASS()
class PARTICLERUSH_API UHoverMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY()

#pragma region OVERRIDES
protected:
	virtual void InitializeComponent() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual float GetMaxSpeed() const override;
#pragma endregion
	

#pragma region PROPERTIES
protected:
	UPROPERTY(EditAnywhere, Meta = (Category = "Movement"))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Meta = (Category = "Movement"))
	float Acceleration;

	UPROPERTY(EditAnywhere, Meta = (Category = "Movement"))
	float Deceleration;

	UPROPERTY(EditAnywhere, Meta = (Category = "Movement"))
	float HoverFriction;

	UPROPERTY(EditAnywhere, Meta = (Category = "Rotation"))
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, Meta = (Category = "Gravity"))
	float GravityForce;

	/* Force to counter gravity, should be Greater than Gravity Force */
	UPROPERTY(EditAnywhere, Meta = (Category = "Gravity"))
	float MaxHoverForce;

	UPROPERTY(EditAnywhere, Meta = (Category = "Gravity"))
	float HoverHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Gravity"))
	float CurrentSurfaceDistance;

	UPROPERTY(EditAnywhere, Meta = (Category = "Jump"))
	float JumpZVelocity;
#pragma endregion
	

#pragma region PROPERTY ACCESSORS
public:
	FRotator GetDeltaRotationRate(float DeltaTime);
#pragma endregion


#pragma region FIELDS
private:
	/** The normalized gravity direction. Need not be the exact direction as Gravity. Maybe affected by grind, etc */
	FVector mGravityDir;

	/* The coefficent m of linear equation f(h) = m * h + b, where h is the required hover height */
	float mHoverForceFallingCoffiecient;

	/* The offset b of linear equation f(h) = m * h + b, where h is the required hover height */
	float mHoverForceFallingOffset;
#pragma endregion


#pragma region PRIVATE METHODS
private:
	/* Consumes the Input Vector specified during the AddMovementInput function call for the pawn.
	*  Based on the input it moves the character forward or backward.
	*/
	void ApplyLinearInput(float DeltaTime);

	/* The forces applied by hover craft based on distance from ground
	* G - F, where
	* G = Gravitational force
	* F = Force based on distance from ground, f(h) = m * h + b
	*/
	FVector ResolveHoverForces(const FVector& InitialVelocity, float DeltaTime);

	/* Compute the distance from the current location to ground */
	float GetDistanceToSurface(const FVector& TestLocation);

	/* Force based on distance from ground, f(h) = m * h + b
	* m = (Gravity - HoverForce) / HoverHeight
	* b = HoverForce
	*/
	float GetHoverForce(const float SurfaceDistance);

	/* Do a simple single sweep test */
	bool SurfaceSweepTest(
		FHitResult& OutHit,
		const FVector& Start,
		const FVector& End,
		ECollisionChannel TraceChannel,
		const struct FCollisionShape& CollisionShape,
		const struct FCollisionQueryParams& Params,
		const struct FCollisionResponseParams& ResponseParam
		) const;
#pragma endregion
};


#pragma region INLINE DEFINITIONS
FORCEINLINE float UHoverMovementComponent::GetMaxSpeed() const
{
	return MaxSpeed;
}

FORCEINLINE FRotator UHoverMovementComponent::GetDeltaRotationRate(float DeltaTime)
{
	return DeltaTime * RotationRate;
}

FORCEINLINE float UHoverMovementComponent::GetHoverForce(const float SurfaceDistance)
{
	return FMath::Max(0.0f, mHoverForceFallingCoffiecient * SurfaceDistance + mHoverForceFallingOffset);
}
#pragma endregion