// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "HoverMovementComponent.h"



UHoverMovementComponent::UHoverMovementComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	mGravityDir = -FVector::UpVector;

	MovementMode = EHoverMode::Hovering;

	ResetMoveState();
}


#pragma region OVERRIDES
void UHoverMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	/* Find the parameters of equation f(h) = m * h + b */
	mHoverForceFallingCoffiecient = (GravityForce - MaxHoverForce) / HoverHeight;
	mHoverForceFallingOffset = MaxHoverForce;
}


void UHoverMovementComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}


void UHoverMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	const AController* Controller = PawnOwner->GetController();
	if (Controller == NULL || !Controller->IsLocalController())
		return;

	// Controller by Player
	if (Controller->IsLocalPlayerController())
	{
		ApplyLinearInput(DeltaTime);
	}
	// if it's not player controller, but we do have a controller, then it's AI
	// and we need to limit the speed
	else if (IsExceedingMaxSpeed(MaxSpeed))
	{
		Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
	}

	// Move actor
	FVector Delta = Velocity * DeltaTime;

	if (!Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FRotator Rotation = UpdatedComponent->GetComponentRotation();
		FVector TraceStart = OldLocation;

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			//HandleImpact(Hit, DeltaTime, Delta);

			// Try to slide the remaining distance along the surface.
			//SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}

		// Update velocity
		// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
		const FVector NewLocation = UpdatedComponent->GetComponentLocation();
		Velocity = ((NewLocation - OldLocation) / DeltaTime);
	}

	// Find Distance
	CurrentSurfaceDistance = GetDistanceToSurface(GetOwner()->GetActorLocation());

	// Apply Gravity
	Velocity = ResolveHoverForces(Velocity, DeltaTime);

	// Finalize Movement
	UpdateComponentVelocity();
}
#pragma endregion


#pragma region PRIVATE METHODS
void UHoverMovementComponent::ApplyLinearInput(float DeltaTime)
{
	/* Fetch the Input Vector*/
	const FVector controlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);
	const float analogInputModifier = (controlAcceleration.SizeSquared() > 0.f ? controlAcceleration.Size() : 0.f);

	/* Adjust Speed based on Input*/
	const float maxPawnSpeed = GetMaxSpeed() * analogInputModifier;
	const bool	bExceedingMaxSpeed = IsExceedingMaxSpeed(maxPawnSpeed);
	const float currentSpeed = Velocity.Size();
	const float currentSpeedSqrd = Velocity.SizeSquared();

	if (analogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (currentSpeedSqrd > 0.f)
		{
			Velocity -= (Velocity - controlAcceleration * currentSpeed) * FMath::Min(DeltaTime * 8.f, 1.f);
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (currentSpeedSqrd > 0.f)
		{
			float speedToApply = 0.0f;

			// Don't allow braking to lower than max speed if we started above it.
			if (bExceedingMaxSpeed && currentSpeedSqrd < FMath::Square(maxPawnSpeed))
			{
				speedToApply = maxPawnSpeed;
			}
			else
			{
				speedToApply = FMath::Max(currentSpeed - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			}
			Velocity = Velocity.GetSafeNormal() *speedToApply;
		}
	}

	// Apply acceleration and clamp velocity magnitude.
	const float newMaxSpeed = (IsExceedingMaxSpeed(maxPawnSpeed)) ? currentSpeed : maxPawnSpeed;

	// Make sure to not exceed max speed
	Velocity += controlAcceleration * FMath::Abs(Acceleration) * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(newMaxSpeed);

	/* Reset Input Vector for further operations*/
	ConsumeInputVector();
}


FVector UHoverMovementComponent::ResolveHoverForces(const FVector& InitialVelocity, float DeltaTime)
{
	FVector Result = InitialVelocity;

	/* Compute Hover Force */
	float hoverForce = (CurrentSurfaceDistance < 0.0f) ? 0.0f : GetHoverForce(CurrentSurfaceDistance);

	/* Net Acceleration = G - F*/
	CurrentAppliedHoverForce = (GravityForce - hoverForce) * DeltaTime;
	FVector netAccelerationPerDeltaTime = mGravityDir * CurrentAppliedHoverForce;

	Result += netAccelerationPerDeltaTime;

	if (CurrentAppliedHoverForce > ForceThreshold)
		MovementMode = EHoverMode::Falling;
	else if (CurrentAppliedHoverForce < -ForceThreshold)
		MovementMode = EHoverMode::Rising;
	else
		MovementMode = EHoverMode::Hovering;

	return Result;
}


float UHoverMovementComponent::GetDistanceToSurface(const FVector& TestLocation)
{
	float distance = -1.0f;
	static const FName QUERY_PARAMS_NAME = "DistToSurfaceQuery";

	FCollisionQueryParams QueryParams(QUERY_PARAMS_NAME, false, GetOwner());
	FCollisionResponseParams ResponseParam;
	InitCollisionParams(QueryParams, ResponseParam);
	const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();

	float SweepDistance = HoverHeight * 3.0f;
	float SweepRadius = 10.0f;

	// Sweep test
	float TraceDist = SweepDistance;

	bool bBlockingHit = false;
	static const FName ComputeSurfaceDistName(TEXT("ComputeSurfaceDistSweep"));
	QueryParams.TraceTag = ComputeSurfaceDistName;
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(SweepRadius, HoverHeight);

	FHitResult Hit(1.f);
	bBlockingHit = SurfaceSweepTest(Hit, TestLocation, TestLocation + FVector(0.f, 0.f, -TraceDist), CollisionChannel, CapsuleShape, QueryParams, ResponseParam);

	if (bBlockingHit)
	{
		distance = (Hit.Location - TestLocation).Size();
	}

	return distance;
}


bool UHoverMovementComponent::SurfaceSweepTest(
	FHitResult& OutHit,
	const FVector& Start,
	const FVector& End,
	ECollisionChannel TraceChannel,
	const struct FCollisionShape& CollisionShape,
	const struct FCollisionQueryParams& Params,
	const struct FCollisionResponseParams& ResponseParam
	) const
{
	bool bBlockingHit = false;

	bBlockingHit = GetWorld()->SweepSingle(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParam);

	return bBlockingHit;
}
#pragma endregion