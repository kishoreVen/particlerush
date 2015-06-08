// Fill out your copyright notice in the Description page of Project Settings.

/* Engine Headers */
#include "ParticleRush.h"
#include "DisplayDebugHelpers.h"

/* Custom Headers */
#include "RushCharacter.h"
#include "RushCharacterMovementComponent.h"
#include "RushCameraComponent.h"
#include "RushCameraArmComponent.h"


ARushCharacter::ARushCharacter(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	#pragma region COMPONENT
	// Create Collision Component
	SphereCollision = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionSphere"));
	if (SphereCollision)
	{
		SphereCollision->InitSphereRadius(48.0f);

		SphereCollision->CanCharacterStepUpOn			= ECB_No;
		SphereCollision->bShouldUpdatePhysicsVolume		= true;
		SphereCollision->bCheckAsyncSceneOnMove			= false;
		SphereCollision->bCanEverAffectNavigation		= false;

		RootComponent = SphereCollision;

		static FName CollisionProfileName(TEXT("Pawn"));
		SphereCollision->SetCollisionProfileName(CollisionProfileName);

		//Set Callbacks
		SphereCollision->OnComponentHit.AddDynamic(this, &ARushCharacter::OnCapsuleCollision);
	}

	// Create a spring arm component
	RushCameraBoom = ObjectInitializer.CreateDefaultSubobject<URushCameraArmComponent>(this, TEXT("RushCameraBoom"));
	if (RushCameraBoom)
	{
		RushCameraBoom->AttachParent = SphereCollision;
	}
	
	// Create camera component 
	RushCamera = ObjectInitializer.CreateDefaultSubobject<URushCameraComponent>(this, TEXT("RushCamera"));
	if (RushCamera)
	{
		RushCamera->AttachTo(RushCameraBoom, USpringArmComponent::SocketName);
		RushCamera->bUsePawnControlRotation = false;
	}

	// Controller Setup
	bUseControllerRotationYaw	= true;
	bUseControllerRotationRoll	= false;
	bUseControllerRotationPitch = false;

	// Create Point Light Component
	RushNavigationLight = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("RushNavigationLight"));
	if (RushNavigationLight)
	{
		RushNavigationLight->AttachParent = RootComponent;
	}

	// Mesh creation
	RushSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RushSkeletalMesh"));
	if (RushSkeletalMesh)
	{
		RushSkeletalMesh->AlwaysLoadOnClient				= true;
		RushSkeletalMesh->AlwaysLoadOnServer				= true;
		RushSkeletalMesh->bOwnerNoSee						= false;
		RushSkeletalMesh->MeshComponentUpdateFlag			= EMeshComponentUpdateFlag::AlwaysTickPose;
		RushSkeletalMesh->bCastDynamicShadow				= true;
		RushSkeletalMesh->bAffectDynamicIndirectLighting	= true;
		RushSkeletalMesh->PrimaryComponentTick.TickGroup	= TG_PrePhysics;
		RushSkeletalMesh->bChartDistanceFactor				= true;
		RushSkeletalMesh->bGenerateOverlapEvents			= false;
		RushSkeletalMesh->bCanEverAffectNavigation			= false;

		RushSkeletalMesh->AttachParent = SphereCollision;

		static FName CollisionProfileName(TEXT("CharacterMesh"));
		RushSkeletalMesh->SetCollisionProfileName(CollisionProfileName);
	}

	// Setup Movement Component
	RushMovementComponent = ObjectInitializer.CreateDefaultSubobject<URushCharacterMovementComponent>(this, TEXT("RushMovementComponent"));
	if (RushMovementComponent)
	{
		RushMovementComponent->UpdatedComponent = SphereCollision;
	}
	#pragma endregion


	#pragma region BEHAVIOR INITIALIZE
	InitializeBehaviorMovement();

	InitializeBehaviorBounce();

	InitializeBehaviorBoost();

	InitializeBehaviorRefraction();
	#pragma endregion


	#pragma region DEBUG SETUP
	_shouldDrawCharacterStats = false;
	_shouldDrawWallCollisionResults = false;
	#pragma endregion
}


#pragma region EXPOSED
USkeletalMeshComponent* ARushCharacter::GetMesh() const
{
	return RushSkeletalMesh;
}

URushCharacterMovementComponent* ARushCharacter::GetRushMovementComponent() const
{
	return RushMovementComponent;
}
#pragma endregion


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

	if ((PropertyName == TEXT("RushData")))
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
			_inputDOFMask = requestedMask;		// Requested everything to be turned on
		else
			_inputDOFMask = 0;					// Requested everything to be turned off
		break;
	default:
		if (enable)
			_inputDOFMask |= requestedMask;		// Eg. 1001 | 0010 = 1011 ... 2nd bit was activated i.e EInputDOF::TURN was activated
		else
			_inputDOFMask &= (~requestedMask);	// Eg. 1011 & (~0010) = 1011 & (1101) = 1001 ... 2nd bit was activated i.e EInputDOF::TURN was deactivated
		break;
	}
}

bool ARushCharacter::IsInputDOFActive(TEnumAsByte<EInputDOF::Type> InputDOF)
{
	int32 maskedInput = _inputDOFMask & (int32)InputDOF; // Eg. 1001 & 0010 = 0000 ... 2nd bit is off mean EInputDOF::TURN was deactivated

	return !(maskedInput == 0); // If the result is not 0, the one bit is active, meaning that state is active
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