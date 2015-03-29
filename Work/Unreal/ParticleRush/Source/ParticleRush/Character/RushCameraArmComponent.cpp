// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCameraArmComponent.h"

/* Engine Headers*/

/* Custom Headers */
#include "Character/RushCharacter.h"

void URushCameraArmComponent::DoCameraLag(float DeltaTime)
{
	ARushCharacter* rush = static_cast<ARushCharacter*>(GetOwner());

	if (rush == NULL)
		return;

	UCharacterMovementComponent* movementComponent = static_cast<UCharacterMovementComponent*>(rush->GetMovementComponent());

	/*float currentArmLength = this->TargetArmLength;
	float mappedSpeedOfRush = movementComponent->Velocity.Size();

	mappedSpeedOfRush = FMath::GetMappedRangeValue(FVector2D(0.0f, movementComponent->GetMaxSpeed()), SpeedImpactOnArmCatchup, mappedSpeedOfRush);

	float targetArmLength = DefaultDistanceFromRush + mappedSpeedOfRush;

	float interolatedArmLength = FMath::FInterpTo(currentArmLength, targetArmLength, DeltaTime, ArmCatchUpSpeed);
	this->TargetArmLength = interolatedArmLength;*/	


	FRotator DesiredRot = GetComponentRotation();	

	// Apply 'lag' to rotation if desired
	DesiredRot = FMath::RInterpTo(PreviousDesiredRot, DesiredRot, DeltaTime, CameraRotationLagSpeed);	
	PreviousDesiredRot = DesiredRot;

	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	// We lag the target, not the actual camera position, so rotating the camera around does not have lag
	FVector DesiredLoc = ArmOrigin;
	
	DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);	

	// Clamp distance if requested
	bool bClampedDist = false;
	if (CameraLagMaxDistance > 0.f)
	{
		const FVector FromOrigin = DesiredLoc - ArmOrigin;
		if (FromOrigin.SizeSquared() > FMath::Square(CameraLagMaxDistance))
		{
			DesiredLoc = ArmOrigin + FromOrigin.GetClampedToMaxSize(CameraLagMaxDistance);
			bClampedDist = true;
		}
	}
	

	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc;
	if (TargetArmLength != 0.0f)
	{
		static FName TraceTagName(TEXT("SpringArm"));
		FCollisionQueryParams QueryParams(TraceTagName, false, GetOwner());

		FHitResult Result;
		GetWorld()->SweepSingle(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

		ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);
	}
	else
	{
		ResultLoc = DesiredLoc;
	}

	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredRot, ResultLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(ComponentToWorld);

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}


void URushCameraArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	DoCameraLag(DeltaTime);
}