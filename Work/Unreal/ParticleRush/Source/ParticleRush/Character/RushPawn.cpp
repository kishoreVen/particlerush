// Fill out your copyright notice in the Description page of Project Settings.

#include "ParticleRush.h"
#include "RushPawn.h"

#pragma region Logging Defines
DEFINE_LOG_CATEGORY_STATIC(RushPawnEngineInfo, Log, All);
#pragma endregion

#pragma region Base Overrides
// Sets default values
ARushPawn::ARushPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Collision Component and set it as root
	RushCapsuleCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RushCollisionComponent"));
	RushCapsuleCollisionComponent->InitCapsuleSize(34.0f, 24.0f);
	RushCapsuleCollisionComponent->bShouldUpdatePhysicsVolume = true;	
	RushCapsuleCollisionComponent->CanCharacterStepUpOn = ECB_No;	
	RushCapsuleCollisionComponent->bCanEverAffectNavigation = false;	
	RushCapsuleCollisionComponent->SetCollisionProfileName(TEXT("RushCollisionProfile"));
	RootComponent = RushCapsuleCollisionComponent;	

	// Setup Movement Component
	RushMovementComponent = CreateDefaultSubobject<URushMovementComponent>(TEXT("RushMovmentComponent"));
	RushMovementComponent->UpdatedComponent = RushCapsuleCollisionComponent;	

	// Establish Rush Skeletal Mesh Component
	RushSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RushSkeletalMeshComponent"));
	RushSkeletalMeshComponent->AttachTo(RushCapsuleCollisionComponent);
	RushSkeletalMeshComponent->AlwaysLoadOnClient = true;
	RushSkeletalMeshComponent->AlwaysLoadOnServer = true;
	RushSkeletalMeshComponent->bOwnerNoSee = false;
	RushSkeletalMeshComponent->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
	RushSkeletalMeshComponent->bCastDynamicShadow = false;
	RushSkeletalMeshComponent->bAffectDynamicIndirectLighting = true;
	RushSkeletalMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	RushSkeletalMeshComponent->bChartDistanceFactor = true;
	RushSkeletalMeshComponent->SetCollisionProfileName(TEXT("CharacterMesh"));
	RushSkeletalMeshComponent->bGenerateOverlapEvents = false;
	RushSkeletalMeshComponent->bCanEverAffectNavigation = false;

	// Create a spring arm component
	RushCameraArmComponent = CreateDefaultSubobject<URushCameraArmComponent>(TEXT("RushCameraArm"));
	RushCameraArmComponent->AttachTo(RushCapsuleCollisionComponent);
	RushCameraArmComponent->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	RushCameraArmComponent->SocketOffset = FVector(0.f, 0.f, 60.f);
	RushCameraArmComponent->bEnableCameraLag = true;
	RushCameraArmComponent->bEnableCameraRotationLag = true;

	// Create camera component 
	RushCameraComponent = CreateDefaultSubobject<URushCameraComponent>(TEXT("RushCamera"));
	RushCameraComponent->AttachTo(RushCameraArmComponent, URushCameraArmComponent::SocketName);
	RushCameraComponent->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Create Point Light Component
	RushNavigationLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RushNavigationLight"));
	RushNavigationLight->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ARushPawn::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ARushPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ARushPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARushPawn::MoveForward);
	InputComponent->BindAxis("TurnRight", this, &ARushPawn::TurnRight);
	InputComponent->BindAxis("SharpTurn", this, &ARushPawn::ActivateSharpTurn);
}

// Return the correct movement component
UPawnMovementComponent* ARushPawn::GetMovementComponent() const
{
	return RushMovementComponent;
}
#pragma endregion


#pragma region Rush Input
void ARushPawn::MoveForward(float value)
{
	if ((Controller == NULL) || (value == 0.0f))
		return;
	
	FVector actorForward = GetActorForwardVector();
	AddMovementInput(actorForward, value);	
}


void ARushPawn::TurnRight(float value)
{
	if (Controller == NULL)
		return;

	AddControllerYawInput(value);		
}


void ARushPawn::ActivateSharpTurn(float value)
{
	/*
	if (Controller == NULL || value == 0.0f)
		return;
	
	_sharpTurnTarget = Controller->GetControlRotation() + FRotator(0.0f, value * 90.0f, 0.0f);
	*/
}
#pragma endregion
