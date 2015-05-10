// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/* Engine Headers */
#include "GameFramework/Character.h"

/* Custom Headers */
#include "RushData.h"
#include "RushFlags.h"

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
#pragma endregion

#pragma region Rush Behaviors
#pragma region Common
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Exposed Data"))
	struct FRushData RushData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Exposed Data"))
	struct FRushFlags RushFlags;
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
#pragma endregion

#pragma region MOVEMENT
private:
	float _targetMeshTurningRollAngle;

	FRotator _defaultMeshRotator;

	void InitializeBehaviorMovement();

	void OnBeginPlayBehaviorMovement();

	void ExecuteMeshRotationPerTick(float deltaSeconds);
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

public:
#pragma endregion

#pragma region BOOST
private:
	float _timeLeftForBoostToEnd;

	float _lastBoostTime;

	int32 _boostChainCounter;

	void InitializeBehaviorBoost();

	void ExecuteBoostPerTick(float deltaSeconds);

	void PerformBoost();
protected:
	/*
	* Turn on the boost state to propel rush at higher speeds
	*/
	UFUNCTION()
	void ActivateBoost();

public:
#pragma endregion

#pragma region BOUNCE
private:
	float		_timeBeforeRegainingControlFromBounce;

	FRotator	_bounceTargetOrientation;

	void InitializeBehaviorBounce();
protected:
	/*
	* Function to perform bounce against the wall
	*/
	void BounceAgainstWall(class AActor* OtherActor, const FHitResult& HitResult);

	void PerformBounce(FVector HitNormal);

	void ExecuteBouncePerTick(float deltaSeconds);

public:
#pragma endregion

#pragma region TIMER
private:
	float _targetRushTimeScale;

	void SetRushTargetTimeScale(float timeScale);

	void ResetRushTimeScale();

	void ExecuteRushTimeScaleUpdatePerTick(float DeltaSeconds);

protected:

public:
#pragma endregion

#pragma region DEBUG
	private:
		bool _shouldDrawWallCollisionResults;

	public:
		UFUNCTION(exec, meta = (FriendlyName = "Particle Rush Console ~ ToggleDrawWallCollisionResults"))
		void ToggleDrawWallCollisionResults();
#pragma endregion
};