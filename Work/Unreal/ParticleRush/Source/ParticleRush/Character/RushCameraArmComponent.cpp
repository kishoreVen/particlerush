// Fill out your copyright notice in the Description page of Project Settings.

/* Engine Headers*/
#include "ParticleRush.h"

/* Custom Headers */
#include "RushCameraArmComponent.h"
#include "Character/RushCharacter.h"
#include "Generic/ParticleRushUtils.h"



void URushCameraArmComponent::InitializeCameraArm(URushCameraComponent* rushCamera)
{
	RushCamera = rushCamera;

	RequestCameraStageSwitch(0);

	RushCamera->InitializeCamera();
}


void URushCameraArmComponent::DoCameraLag(float DeltaTime)
{
	ARushCharacter* rush = static_cast<ARushCharacter*>(GetOwner());

	if (rush == NULL)
		return;

	UCharacterMovementComponent* movementComponent = static_cast<UCharacterMovementComponent*>(rush->GetMovementComponent());

	FRotator DesiredRot = GetComponentRotation();	

	// Apply 'lag' to rotation if desired
	
	DesiredRot = UParticleRushUtils::ROverShootingInterpTo(PreviousDesiredRot, DesiredRot, DeltaTime, CameraRotationLagSpeed);
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
		GetWorld()->SweepSingleByChannel(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

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


void URushCameraArmComponent::UpdateCameraToReachSwitchTarget(float DeltaTime)
{
	if (!EnableCameraSwitching)
		return;

	float interpTargetArmLength = FMath::FInterpTo(TargetArmLength, _targetTargetArmLength, DeltaTime, CameraSwitchBlendTime);
	FRotator currentTargetArmRotation = RelativeRotation;
	FRotator interpTargetArmRotation = FMath::RInterpTo(currentTargetArmRotation, _targetTargetArmRotation, DeltaTime, CameraSwitchBlendTime);

	TargetArmLength = interpTargetArmLength;
	RelativeRotation = interpTargetArmRotation;

	RushCamera->RotateCameraToStoredTarget(DeltaTime);
}

void URushCameraArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	DoCameraLag(DeltaTime);

	UpdateCameraToReachSwitchTarget(DeltaTime);

	RushCamera->RotateCameraToRightStickTarget(DeltaTime);
}


void URushCameraArmComponent::RequestCameraStageSwitch()
{
	if (RushCamera == NULL || CameraSwitchTransforms.Num() == 0)
		return;

	int32 stage = _currentCameraSwitchStage;
	stage = (stage + 1) % (CameraSwitchTransforms.Num());
	
	RequestCameraStageSwitch(stage);
}


void URushCameraArmComponent::RequestCameraStageSwitch(int32 stage)
{
	if (RushCamera == NULL || CameraSwitchTransforms.Num() == 0)
		return;

	_currentCameraSwitchStage = stage;

	FCameraDataVector requestValue = CameraSwitchTransforms[_currentCameraSwitchStage];
	_targetTargetArmLength = requestValue.TargetArmLength;
	_targetTargetArmRotation = requestValue.TargetArmRotation;

	RushCamera->RequestCameraStageSwitch(requestValue.CameraRotation, CameraSwitchBlendTime);
}