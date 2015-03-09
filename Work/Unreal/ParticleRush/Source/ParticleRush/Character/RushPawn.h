// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "GameFramework/Pawn.h"

/* Custom Headers */
#include "Character/RushMovementComponent.h"
#include "Character/RushCameraArmComponent.h"
#include "Character/RushCameraComponent.h"

/* Generated Headers */
#include "RushPawn.generated.h"

UCLASS(Blueprintable)
class PARTICLERUSH_API ARushPawn : public APawn
{
	GENERATED_BODY()

#pragma region Component Declarations
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Collision"))
	class UCapsuleComponent*				RushCapsuleCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Collision"))
	class USkeletalMeshComponent*			RushSkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Camera"))
	class URushCameraArmComponent*			RushCameraArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Camera"))
	class URushCameraComponent*				RushCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Movement"))
	class URushMovementComponent*			RushMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Lighting"))
	class UPointLightComponent*				RushNavigationLight;
#pragma endregion

public:
	// Sets default values for this pawn's properties
	ARushPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
