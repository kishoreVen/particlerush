// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "GameFramework/Character.h"

/* Custom Headers */
#include "HeroState.h"
#include "HeroData.h"
#include "RushCameraComponent.h"
#include "RushCameraArmComponent.h"

/* Defines */
#include "Generic/ParticleRushDefines.h"

/* Generated Headers */
#include "RushCharacter.generated.h"


/**
 * The gameplay class for the main character "Rush"
 */
UCLASS(Blueprintable)
class PARTICLERUSH_API ARushCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Component Declarations
public:
	ARushCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Camera"))
	class URushCameraArmComponent* RushCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Camera"))
	class URushCameraComponent* RushCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Action"))
	class USphereComponent* RushActionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Lighting"))
	class UPointLightComponent* RushNavigationLight;
#pragma endregion

#pragma region Base Class Overrides
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
#pragma endregion

#pragma region Physics Methods and Callbacks
	UFUNCTION()
	void OnCapsuleCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION()
	void OnRushActionSphereBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRushActionSphereEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

#pragma region Rush Input
protected:
	/* Moves the character forward based on the Actor's world forward vector
	* Value > 0.0f - Moves forward
	* Value < 0.0f - Moves Backward
	*/
	UFUNCTION()
	void MoveForward(float value);

	/*
	* Turns the character right based on the Turn speed
	* value > 0.0f - Turns Right
	* value < 0.0f - Turns Left
	*/
	UFUNCTION()
	void TurnRight(float value);

	/*
	* Turn on the boost state to propel rush at higher speeds
	*/
	UFUNCTION()
	void ActivateBoost();

	/*
	* Turn 90 degrees to execute sharp turn
	*/
	UFUNCTION()
	void ActivateSharpTurn(float value);

	/* 
	* Hard Stop with 180 degree turn 
	*/
	UFUNCTION()
	void ActivateHardStop();


	/*
	* Function to perform bounce against the wall
	*/
	void BounceAgainstWall(const FHitResult& HitResult);
#pragma endregion

#pragma region State Management
protected:
	/** Function that will check if states that require ticking are active and execute them */
	void ExecuteActiveStateTicks(float DeltaSeconds);

	/** Contains a composite score of all active states */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Character State Management"))
	int32 currentlyActiveStates;

public:
	/** Activates the given state */
	void ActivateState(EHeroState::Type turnOnState);

	/** De-Activates the given state */
	void DeactivateState(EHeroState::Type turnOffState);

	/** Checks if a given state is active */
	bool IsStateActive(EHeroState::Type checkState);
#pragma endregion

#pragma region Rush Behaviors
	#pragma region Common
	private:
		void UpdateMovementComponentParameters();

	public:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Data"))
		struct FHeroData HeroData;
	#pragma endregion

	#pragma region Turning
private:
	float _targetMeshTurningRollAngle;

	void ExecuteMeshRotationPerTick(float deltaSeconds);

protected:	
	/* How fast the Mesh should rotate in-order to achieve MeshMaxPitchAngle */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Behavior - Turning"))
	float MeshTurningRollSpeed;

	/* The Maximum Roll the Mesh can achieve while turning */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Behavior - Turning"))
	float MeshTurningMaxRollAngle;
	#pragma endregion

	#pragma region Boost
private:
	float _timeLeftForBoostToEnd;

	float _lastBoostTime;

	int32 _boostChainCounter;

	void ExecuteBoostPerTick(float deltaSeconds);

	void PerformBoost();
protected:	
	#pragma endregion

	#pragma region Sharp Turn
private:
	FRotator _sharpTurnTarget;

	void ExecuteSharpTurnPerTick(float deltaSeconds);
protected:
	#pragma endregion

	#pragma region HardStop
private:
	float _timeLeftForHardStopToEnd;

	FRotator _hardTurnTarget;
protected:
	void ExecuteHardStopPerTick(float delatSeconds);
	#pragma endregion

	#pragma region Bounce
private:
	float		_timeBeforeRegainingControlFromBounce;

	FRotator	_bounceTargetOrientation;
protected:

	void ExecuteBouncePerTick(float deltaSeconds);
	/* End Bounce*/
	#pragma endregion
#pragma endregion

#pragma region Rush Action Sphere Timer Management
private:
	float _targetRushTimeScale;

	void SetRushTargetTimeScale(float timeScale);

	void ResetRushTimeScale();

	void ExecuteRushTimeScaleUpdatePerTick(float DeltaSeconds);
#pragma endregion

#pragma region Debug Section
	private:
		bool _shouldDrawWallCollisionResults;

	public:
		UFUNCTION(exec, meta = (FriendlyName = "Particle Rush Console ~ ToggleDrawWallCollisionResults"))
		void ToggleDrawWallCollisionResults();
#pragma endregion
};