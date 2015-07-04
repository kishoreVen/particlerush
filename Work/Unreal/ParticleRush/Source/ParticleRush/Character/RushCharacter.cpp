// Fill out your copyright notice in the Description page of Project Settings.

/* Engine Headers */
#include "ParticleRush.h"
#include "DisplayDebugHelpers.h"

/* Custom Headers */
#include "RushCharacter.h"
#include "RushCharacterMovementComponent.h"
#include "RushCameraComponent.h"
#include "RushCameraArmComponent.h"


ARushCharacter* ARushCharacter::mRushInstance = NULL;

ARushCharacter::ARushCharacter(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<URushCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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


#pragma region Behavior Parameter Setups
	InitializeBehaviorMovement();

	InitializeBehaviorBounce();

	InitializeBehaviorBoost();

	InitializeBehaviorRefraction();
#pragma endregion


#pragma region DEBUG SETUP
	_shouldDrawCharacterStats = false;
	_shouldDrawWallCollisionResults = false;
#pragma endregion


#pragma region Rush Action Sphere Timer Management
	ResetRushTimeScale();
#pragma endregion


#pragma region Instance Setup
	if (mRushInstance == NULL)
		mRushInstance = this;
#pragma endregion
}


#pragma region OVERRIDES
void ARushCharacter::BeginPlay()
{
#pragma region OnBegin Behavior Setup
	OnBeginPlayBehaviorMovement();
#pragma endregion

	Super::BeginPlay();
}


void ARushCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RushCameraBoom->InitializeCameraArm(RushCamera);
}


void ARushCharacter::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UProperty* MemberProperty = PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue();
	FName PropertyName = (MemberProperty != NULL) ? MemberProperty->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ARushCharacter, RushData))
	{
		RushData.UpdateProperty(PropertyChangedEvent);
	}

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}


void ARushCharacter::Tick(float DeltaTime)
{
	//ExecuteRushTimeScaleUpdatePerTick(DeltaSeconds);

	ExecuteBoostPerTick(DeltaTime);

	ExecuteBouncePerTick(DeltaTime);

	ExecuteRefractionPerTick(DeltaTime);

	ExecuteMeshRotationPerTick(DeltaTime);

	ExecuteSharpTurnPerTick(DeltaTime);

	#pragma region DEBUG UPDATE
	if (_shouldDrawCharacterStats)
		DrawCharacterStats();
	#pragma endregion

	Super::Tick(DeltaTime);
}


void ARushCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	/* Enable All Degrees of Freedom. 
	*  TODO: If we have more complicated features that turn on for specific conditions, enable only those.
	*/
	SetInputDOFState(EInputDOF::EVERYTHING, true);

	InputComponent->BindAxis("MoveForward", this, &ARushCharacter::MoveForward);
	InputComponent->BindAxis("TurnRight", this, &ARushCharacter::TurnRight);
	
	InputComponent->BindAxis("SharpTurn", this, &ARushCharacter::ActivateSharpTurn);

	InputComponent->BindAxis("Brake", this, &ARushCharacter::ApplyBraking);

	InputComponent->BindAction("Boost", IE_Pressed, this, &ARushCharacter::ActivateBoost);
	
	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ARushCharacter::SwitchCamera);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ARushCharacter::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ARushCharacter::StopJump);

	InputComponent->BindAxis("CameraYaw", this, &ARushCharacter::TurnCameraYaw);
	InputComponent->BindAxis("CameraRoll", this, &ARushCharacter::TurnCameraRoll);
}
#pragma endregion


#pragma region PHYSICS
void ARushCharacter::OnCapsuleCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	BounceAgainstObstacle(OtherActor, HitResult);

	RefractAgainstObstacle(OtherActor, HitResult);
}

void ARushCharacter::OnRushActionSphereBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ARushCharacter::OnRushActionSphereEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
#pragma region Has Collision with Wall Ended?

#pragma endregion
}
#pragma endregion


#pragma region CAMERA
void ARushCharacter::SwitchCamera()
{
	RushCameraBoom->RequestCameraStageSwitch();
}

void ARushCharacter::TurnCameraYaw(float value)
{
	RushCameraBoom->TurnCameraYaw(value);
}

void ARushCharacter::TurnCameraRoll(float value)
{
	RushCameraBoom->TurnCameraRoll(value);
}
#pragma endregion


#pragma region INPUT
void ARushCharacter::SetInputDOFState(TEnumAsByte<EInputDOF::Type> InputDOF, bool enable)
{
	int32 requestedMask = (int32)InputDOF;
	switch (InputDOF)
	{	
	case EInputDOF::EVERYTHING:
		requestedMask = requestedMask - 1;		// We care about every enum before everything ( 2 ^ n - 1)
		if (enable)
		{
			_inputDOFMask = requestedMask;		// Requested everything to be turned on
		}
		else
		{
			_lastDisableDOFMask = _inputDOFMask;
			_inputDOFMask = 0;					// Requested everything to be turned off
		}	
		break;
	default:
		if (enable)
			_inputDOFMask |= requestedMask;		// Eg. 1001 | 0010 = 1011 ... 2nd bit was activated i.e EInputDOF::TURN was activated
		else
			_inputDOFMask &= (~requestedMask);	// Eg. 1011 & (~0010) = 1011 & (1101) = 1001 ... 2nd bit was activated i.e EInputDOF::TURN was deactivated
		break;
	}
}

void ARushCharacter::ResetInputDOFStateToLastDisableMask()
{
	_inputDOFMask = _lastDisableDOFMask;
}

bool ARushCharacter::IsInputDOFActive(TEnumAsByte<EInputDOF::Type> InputDOF)
{
	int32 maskedInput = _inputDOFMask & (int32)InputDOF; // Eg. 1001 & 0010 = 0000 ... 2nd bit is off mean EInputDOF::TURN was deactivated

	return !(maskedInput == 0); // If the result is not 0, the one bit is active, meaning that state is active
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


#pragma region DEBUG METHODS
void ARushCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	static FName NAME_Rush = FName(TEXT("RUSH"));

	UFont* RenderFont = GEngine->GetSmallFont();

	if (DebugDisplay.IsDisplayOn(NAME_Rush))
	{
		FString TargetDesc = FString::Printf(TEXT("  Rush Flags: [%f | %f | %d | %d]"),
			RushFlags.MomentumPercentage, RushFlags.MomentumDiffPercentage, RushFlags.ChainBoostStage, RushFlags.ChainBounceStage);

		Canvas->DrawText(RenderFont, TargetDesc, 4.0f, YPos);
		YPos += YL;
	}

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);
}

void ARushCharacter::DrawCharacterStats()
{
	if (!GEngine)
		return;
}

void ARushCharacter::ToggleDrawWallCollisionResults()
{
	_shouldDrawWallCollisionResults = !_shouldDrawWallCollisionResults;
}

void ARushCharacter::ToggleDrawCharacterStats()
{
	_shouldDrawCharacterStats = !_shouldDrawCharacterStats;
}
#pragma endregion


#pragma region STATIC
ARushCharacter* ARushCharacter::GetRush()
{
	return mRushInstance;
}
#pragma endregion