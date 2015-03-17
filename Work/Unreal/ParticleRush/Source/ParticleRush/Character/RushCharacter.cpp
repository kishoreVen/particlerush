// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushCharacter.h"
#include "LevelAssets/Wall/Wall.h"
#include "Generic/Utilities.h"


ARushCharacter::ARushCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#pragma region Component Definitions
	// Create Capsule Component
	UCapsuleComponent* capsuleComponent = GetCapsuleComponent();
	capsuleComponent->InitCapsuleSize(34.0f, 48.0f);
	capsuleComponent->OnComponentHit.AddDynamic(this, &ARushCharacter::OnCapsuleCollision);

	// Create a spring arm component
	RushCameraBoom = ObjectInitializer.CreateDefaultSubobject<URushCameraArmComponent>(this, TEXT("RushCameraBoom"));
	RushCameraBoom->AttachTo(capsuleComponent);
	
	// Create camera component 
	RushCamera = ObjectInitializer.CreateDefaultSubobject<URushCameraComponent>(this, TEXT("RushCamera"));
	RushCamera->AttachTo(RushCameraBoom, USpringArmComponent::SocketName);
	RushCamera->bUsePawnControlRotation = false;

	// Create action sphere component
	RushActionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RushActionSphere"));
	RushActionSphere->AttachTo(RootComponent);
	RushActionSphere->OnComponentBeginOverlap.AddDynamic(this, &ARushCharacter::OnRushActionSphereBeginOverlap);
	RushActionSphere->OnComponentEndOverlap.AddDynamic(this, &ARushCharacter::OnRushActionSphereEndOverlap);

	// Create Point Light Component
	RushNavigationLight = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("RushNavigationLight"));
	RushNavigationLight->AttachTo(RootComponent);
#pragma endregion
	
#pragma region State Management Setup
	currentlyActiveStates = (int32)0;
#pragma endregion

#pragma region Behavior Parameter Setups
	#pragma region Turning
	_targetMeshTurningRollAngle = 0.0f;
	MeshTurningRollSpeed		= 35.0f;
	MeshTurningMaxRollAngle		= 35.0f;
	#pragma endregion

	#pragma region Boost
	_timeLeftForBoostToEnd = -1.0f;
	_boostChainCounter = 0;
	#pragma endregion

	#pragma region Sharp Turn
	_sharpTurnTarget = FRotator(0.0f, 0.0f, 0.0f);
	#pragma endregion

	#pragma region Hard Stop
	_timeLeftForHardStopToEnd = -1.0f;
	_hardTurnTarget = FRotator(0.0f, 0.0f, 0.0f);
	#pragma endregion

	#pragma region Bounce
	_timeBeforeRegainingControlFromBounce = -1.0f;
	_bounceTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
	#pragma endregion
#pragma endregion

#pragma region Rush Action Sphere Timer Management
	ResetRushTimeScale();
#pragma endregion

#pragma region Debug Section
	_shouldDrawWallCollisionResults = false;
#pragma endregion
}


#pragma region Base Class Overrides
void ARushCharacter::BeginPlay()
{
	#pragma region OnBegin Setup
	ActivateState(EHeroState::Walk);
	#pragma endregion

	Super::BeginPlay();
}


void ARushCharacter::Tick(float DeltaSeconds)
{
	ExecuteActiveStateTicks(DeltaSeconds);

	//ExecuteCameraParameterBlendPerTick(DeltaSeconds);

	ExecuteRushTimeScaleUpdatePerTick(DeltaSeconds);

	Super::Tick(DeltaSeconds);
}


void ARushCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARushCharacter::MoveForward);
	InputComponent->BindAxis("TurnRight", this, &ARushCharacter::TurnRight);
	InputComponent->BindAxis("SharpTurn", this, &ARushCharacter::ActivateSharpTurn);
	InputComponent->BindAction("HardStop", IE_Pressed, this, &ARushCharacter::ActivateHardStop);
	InputComponent->BindAction("Boost", IE_Pressed, this, &ARushCharacter::ActivateBoost);

}
#pragma endregion


#pragma region Physics Methods and Callbakcs
void ARushCharacter::OnCapsuleCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	#pragma region Is Collision with Wall?
	AWall* collidedWall = dynamic_cast<AWall*>(OtherActor);

	if (collidedWall != NULL)
	{
		BounceAgainstWall(HitResult);
	}
	#pragma endregion
}

void ARushCharacter::OnRushActionSphereBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	#pragma region Is Collision with Wall?

	#pragma endregion
}

void ARushCharacter::OnRushActionSphereEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
#pragma region Has Collision with Wall Ended?

#pragma endregion
}
#pragma endregion


#pragma region State Management Methods
void ARushCharacter::ExecuteActiveStateTicks(float DeltaSeconds)
{
	if (IsStateActive(EHeroState::Walk))
	{
		//ExecuteMeshRotationPerTick(DeltaSeconds);
	}

	if (IsStateActive(EHeroState::Boost))
	{
		ExecuteBoostPerTick(DeltaSeconds);
	}

	if (IsStateActive(EHeroState::SharpTurn))
	{
		ExecuteSharpTurnPerTick(DeltaSeconds);
	}

	if (IsStateActive(EHeroState::HardStop))
	{
		ExecuteHardStopPerTick(DeltaSeconds);
	}

	if (IsStateActive(EHeroState::Bounce))
	{
		ExecuteBouncePerTick(DeltaSeconds);
	}
}


void ARushCharacter::ActivateState(EHeroState::Type turnOnState)
{
	currentlyActiveStates |= (1 << turnOnState);
/*
	UpdateCameraComponentParameters();
	UpdateMovementComponentParameters();*/
}


void ARushCharacter::DeactivateState(EHeroState::Type turnOffState)
{
	int32 max = (1 << EHeroState::Everything) - 1;

	max ^= (1 << turnOffState);

	currentlyActiveStates &= max;

	/*UpdateCameraComponentParameters();
	UpdateMovementComponentParameters();*/
}


bool ARushCharacter::IsStateActive(EHeroState::Type checkState)
{
	return !((currentlyActiveStates & (1 << checkState)) == 0);
}

#pragma endregion


#pragma region Rush Input
void ARushCharacter::MoveForward(float value)
{
	if (	!IsStateActive(EHeroState::Walk)
		||  IsStateActive(EHeroState::HardStop)
		)
		return;

	if ((Controller != NULL) && (value != 0.0f))
	{
		FVector actorForward = GetActorForwardVector();
		AddMovementInput(actorForward, value);		
	}
}


void ARushCharacter::TurnRight(float value)
{
	if (	IsStateActive(EHeroState::SharpTurn)
		||  IsStateActive(EHeroState::HardStop)
		||	IsStateActive(EHeroState::Bounce)
		)
		return;

	if (IsStateActive(EHeroState::Walk))
	{
		if (Controller != NULL)
		{
			AddControllerYawInput(value);
		}
	}	

	/* Mesh Rotation for smooth mesh movement */
	_targetMeshTurningRollAngle = -value * MeshTurningRollSpeed;
}


void ARushCharacter::ActivateBoost()
{
	if (!IsStateActive(EHeroState::Walk))
	{
		return;
	}

	PerformBoost();
}


void ARushCharacter::ActivateSharpTurn(float value)
{
	if (	IsStateActive(EHeroState::SharpTurn) 
		||	IsStateActive(EHeroState::HardStop) 
		||	IsStateActive(EHeroState::Bounce)
		)
		return;

	if (Controller != NULL && value != 0.0f && _sharpTurnTarget.IsNearlyZero())
	{
		_sharpTurnTarget = Controller->GetControlRotation() + FRotator(0.0f, value * 90.0f, 0.0f);
		
		ActivateState(EHeroState::SharpTurn);
	}
}


void ARushCharacter::ActivateHardStop()
{
	if (	IsStateActive(EHeroState::SharpTurn) 
		||	IsStateActive(EHeroState::HardStop) 
		||	IsStateActive(EHeroState::Bounce)
		||  IsStateActive(EHeroState::Air)
		)
		return;

	_timeLeftForHardStopToEnd = HeroData.HardStopDriftDuration;
	_hardTurnTarget = GetController()->GetControlRotation() + FRotator(0.0f, 180.0f, 0.0f);
	ActivateState(EHeroState::HardStop);
}


void ARushCharacter::BounceAgainstWall(const FHitResult& HitResult)
{
	/* Check if it rush is bouncing */
	if (IsStateActive(EHeroState::Bounce))
		return;

	DeactivateState(EHeroState::Walk);
	ActivateState(EHeroState::Bounce);
	_timeBeforeRegainingControlFromBounce = HeroData.BounceDuration;

	FVector rushHeading = GetActorForwardVector();
	rushHeading.Z = 0.0f;

	FVector normal = HitResult.Normal;
	float normalZ = normal.Z;
	normal.Z = 0.0f;
	
	FVector bounceDirection = ParticleRush::Utilities::GetReflectionVector(rushHeading, normal);
	
	_bounceTargetOrientation = bounceDirection.Rotation();

	UCharacterMovementComponent* movementComponent = GetCharacterMovement();
	float jumpFactor = (movementComponent->Velocity.Size() / movementComponent->MaxWalkSpeed);
	float bounceFactor = HeroData.BounceStrength.DataValue2 * jumpFactor;
	bounceDirection.Z = jumpFactor * HeroData.BounceJumpFactor; //* normalZ;

	bounceFactor = FMath::Max<float>(bounceFactor, HeroData.BounceStrength.DataValue1);

	movementComponent->AddImpulse(bounceDirection * bounceFactor, true);

	if (_shouldDrawWallCollisionResults)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, (bounceDirection * bounceFactor).ToString());
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + bounceDirection * 50.0f, 5.0f, FColor::Red, false, 1.0f);
	}
}
#pragma endregion


#pragma region Rush Behaviors
void ARushCharacter::UpdateMovementComponentParameters()
{
	UCharacterMovementComponent* movementComponent = GetCharacterMovement();

	if (IsStateActive(EHeroState::HardStop))
	{
		movementComponent->MaxAcceleration = HeroData.WalkAcceleration;
		movementComponent->MaxWalkSpeed = HeroData.WalkSpeed;
		movementComponent->BrakingDecelerationWalking = HeroData.WalkDeceleration  * HeroData.HardStopBreakingFactor;
		movementComponent->GroundFriction = HeroData.HardStopGroundFriction;

		return;
	}

	if (IsStateActive(EHeroState::Bounce))
	{
		movementComponent->MaxAcceleration = HeroData.WalkAcceleration * HeroData.BounceFactor;
		movementComponent->MaxWalkSpeed = HeroData.WalkSpeed * HeroData.BounceFactor;
		movementComponent->BrakingDecelerationWalking = HeroData.WalkDeceleration / (HeroData.BounceFactor * HeroData.BounceFactor);
		movementComponent->GroundFriction = 0.0f;

		return;
	}

	if (IsStateActive(EHeroState::Boost))
	{
		movementComponent->MaxAcceleration = HeroData.WalkAcceleration * HeroData.BoostFactor;
		movementComponent->MaxWalkSpeed = HeroData.WalkSpeed * HeroData.BoostFactor;
		movementComponent->BrakingDecelerationWalking = HeroData.WalkDeceleration / HeroData.BoostFactor;
		movementComponent->GroundFriction = HeroData.WalkGroundFriction / HeroData.BoostFactor;

		return;
	}

	if (IsStateActive(EHeroState::Walk))
	{
		movementComponent->MaxAcceleration = HeroData.WalkAcceleration;
		movementComponent->MaxWalkSpeed = HeroData.WalkSpeed;
		movementComponent->BrakingDecelerationWalking = HeroData.WalkDeceleration;
		movementComponent->GroundFriction = HeroData.WalkGroundFriction;

		return;
	}
}


void ARushCharacter::ExecuteMeshRotationPerTick(float deltaSeconds)
{
	float currentMeshRoll = GetMesh()->RelativeRotation.Roll;

	if (_targetMeshTurningRollAngle == 0.0f)
	{
		if (currentMeshRoll != 0.0f)
		{
			/* Rotate Back to Position */
			_targetMeshTurningRollAngle = (-currentMeshRoll * MeshTurningRollSpeed * 2.0f) / MeshTurningMaxRollAngle;

			FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
			DeltaRotation.Roll = _targetMeshTurningRollAngle * deltaSeconds;
			GetMesh()->AddLocalRotation(DeltaRotation);
		}		
		
		return;
	}

	if (currentMeshRoll > MeshTurningMaxRollAngle || currentMeshRoll < -MeshTurningMaxRollAngle)
	{
		return;
	}

	FRotator DeltaRotation(0.0f, 0.0f, 0.0f);
	DeltaRotation.Roll = _targetMeshTurningRollAngle * deltaSeconds;

	GetMesh()->AddLocalRotation(DeltaRotation);
}


void ARushCharacter::ExecuteBoostPerTick(float deltaSeconds)
{
	_timeLeftForBoostToEnd -= deltaSeconds;

	if (_timeLeftForBoostToEnd < 0.0f)
	{
		DeactivateState(EHeroState::Boost);

		_timeLeftForBoostToEnd = -1.0f;
	}	
}


void ARushCharacter::PerformBoost()
{
	float currentBoostTime = GetWorld()->GetTimeSeconds();

	if (_boostChainCounter != 0)
	{
		if (currentBoostTime - _lastBoostTime > HeroData.BoostChainResetDuration)
		{
			_boostChainCounter = 0;
		}
	}

	_lastBoostTime = currentBoostTime + HeroData.BoostDuration;

	UCharacterMovementComponent* movementComponent = GetCharacterMovement();

	FVector forwardVector = GetActorForwardVector();

	/* Action Based on Counter */
	switch (_boostChainCounter)
	{
	case 0:
		movementComponent->AddImpulse(forwardVector * 500.0f, true);
		break;
	case 1:
		// Regular Boost
		movementComponent->AddImpulse(forwardVector * 1000.0f, true);
		break;
	case 2:
		// Super Boost - Time Warp
		movementComponent->AddImpulse(forwardVector * 2000.0f, true);
		break;
	case 3:
		movementComponent->AddImpulse(forwardVector * 4000.0f, true);
		// Super Boost - Shoot To Air
		break;
	default:
		break;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::FromInt(_boostChainCounter));

	_boostChainCounter = (_boostChainCounter + 1) % 4;
	_timeLeftForBoostToEnd = HeroData.BoostDuration;
	ActivateState(EHeroState::Boost);
}


void ARushCharacter::ExecuteSharpTurnPerTick(float deltaSeconds)
{
	if (_sharpTurnTarget == FRotator(0.0f, 0.0f, 0.0f))
		return;

	if (Controller != NULL)
	{
		FRotator currentControllerRotation = Controller->GetControlRotation();
		FRotator difference = _sharpTurnTarget - currentControllerRotation;

		if (difference.IsNearlyZero())
		{
			DeactivateState(EHeroState::SharpTurn);
			_sharpTurnTarget = FRotator(0.0f, 0.0f, 0.0f);
		}
		else
		{
			FRotator interpControllerRotation = FMath::RInterpTo(currentControllerRotation, _sharpTurnTarget, deltaSeconds, HeroData.SharpTurnStrength);
			Controller->SetControlRotation(interpControllerRotation);
		}
	}
}


void ARushCharacter::ExecuteHardStopPerTick(float deltaSeconds)
{
	if (_hardTurnTarget == FRotator(0.0f, 0.0f, 0.0f))
		return;

	if (Controller != NULL)
	{
		FRotator currentControllerRotation = Controller->GetControlRotation();
		FRotator difference = _hardTurnTarget - currentControllerRotation;

		if (difference.IsNearlyZero())
		{
			_timeLeftForHardStopToEnd -= deltaSeconds;

			if (_timeLeftForHardStopToEnd < 0.0f)
			{
				DeactivateState(EHeroState::HardStop);

				_timeLeftForBoostToEnd = -1.0f;
				_hardTurnTarget = FRotator(0.0f, 0.0f, 0.0f);
			}
		}
		else
		{
			FRotator interpControllerRotation = FMath::RInterpTo(currentControllerRotation, _hardTurnTarget, deltaSeconds, HeroData.HardStopOrientationStrength);
			Controller->SetControlRotation(interpControllerRotation);
		}
	}
}


void ARushCharacter::ExecuteBouncePerTick(float deltaSeconds)
{
	if (_timeBeforeRegainingControlFromBounce == -1.0f)
	{
		return;
	}

	_timeBeforeRegainingControlFromBounce -= deltaSeconds;

	if (_timeBeforeRegainingControlFromBounce < 0.0f)
	{
		ActivateState(EHeroState::Walk);
		DeactivateState(EHeroState::Bounce);

		_timeBeforeRegainingControlFromBounce = -1.0f;
		_bounceTargetOrientation = FRotator(0.0f, 0.0f, 0.0f);
	}
	else
	{
		if (Controller != NULL)
		{
			FRotator interpRotation = FMath::RInterpTo(Controller->GetControlRotation(), _bounceTargetOrientation, deltaSeconds, HeroData.BounceOrientationStrength);
			Controller->SetControlRotation(interpRotation);
		}
	}
}
#pragma endregion


#pragma region Rush Action Sphere Timer Management
void ARushCharacter::SetRushTargetTimeScale(float timeScale)
{
	_targetRushTimeScale = timeScale;
}

void ARushCharacter::ResetRushTimeScale()
{
	_targetRushTimeScale = 1.0f;
}

void ARushCharacter::ExecuteRushTimeScaleUpdatePerTick(float DeltaSeconds)
{
	if (_targetRushTimeScale == -1.0f)
		return;

	UWorld* world = GetWorld();
	if (world != NULL)
	{
		float timeScale = FMath::FInterpTo(UGameplayStatics::GetGlobalTimeDilation(world), _targetRushTimeScale, DeltaSeconds, 8.0f);

		if (timeScale == _targetRushTimeScale)
		{
			_targetRushTimeScale = -1.0f;
		}

		UGameplayStatics::SetGlobalTimeDilation(world, timeScale);
	}
}
#pragma endregion


#pragma region Debug Section
void ARushCharacter::ToggleDrawWallCollisionResults()
{
	_shouldDrawWallCollisionResults = !_shouldDrawWallCollisionResults;
}
#pragma endregion