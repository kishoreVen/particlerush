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


#pragma region COMPONENTS
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


#pragma region EXPOSED
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Exposed Data"))
	struct FRushData RushData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Rush Exposed Data"))
	struct FRushFlags RushFlags;
#pragma endregion


#pragma region OVERRIDES
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#pragma endregion


#pragma region PHYSICS
	UFUNCTION()
	void OnCapsuleCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UFUNCTION()
	void OnRushActionSphereBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRushActionSphereEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion


#pragma region BRAKING
protected:
	/*
	* Intuitive stopping that increase ground friction and braking deceleration
	*/
	UFUNCTION()
	void ApplyBraking(float value);
#pragma endregion


#pragma region MOVEMENT
private:
	float _targetMeshTurningRollAngle;

	FRotator _defaultMeshRotator;

	FRotator _sharpTurnTarget;

	void InitializeBehaviorMovement();

	void OnBeginPlayBehaviorMovement();

	void ExecuteMeshRotationPerTick(float DeltaTime);

	void ExecuteSharpTurnPerTick(float DeltaTime);
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
	* Turn 90 degrees to execute sharp turn
	*/
	UFUNCTION()
	void ActivateSharpTurn(float value);

public:
#pragma endregion


#pragma region JUMP
private:
	void InitializeBehaviorJump();

protected:
	/* Start a Jump
	*/
	UFUNCTION()
	void StartJump();

	/* Stop a Jump
	*/
	UFUNCTION()
	void StopJump();
#pragma endregion


#pragma region BOOST
private:
	float _timeLeftForBoostToEnd;

	float _lastBoostActvationTime;

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
	bool BounceAgainstObstacle(class AActor* OtherActor, const FHitResult& HitResult);

	void PerformBounce(FVector HitNormal, float OverrideZImpulseFactor);

	void ExecuteBouncePerTick(float deltaSeconds);

public:
#pragma endregion


#pragma region REFRACT
private:
	float						_timeBeforeRegainingControlFromRefraction;

	FRotator					_refractTargetOrientation;

	class ARefractObstacle*		_localRefractionCache;

	void InitializeBehaviorRefraction();
protected:
	/*
	* Function to perform bounce against the wall
	*/
	bool RefractAgainstObstacle(class AActor* OtherActor, const FHitResult& HitResult);

	void PerformRefraction(FVector HitNormal, float RefractiveIndex);

	void ExecuteRefractionPerTick(float deltaSeconds);

public:
#pragma endregion


#pragma region CAMERA
	UFUNCTION()
	void SwitchCamera();

	UFUNCTION()
	void TurnCameraYaw(float value);

	UFUNCTION()
	void TurnCameraRoll(float value);
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


#pragma region EVENTS
public:
	UFUNCTION(BlueprintImplementableEvent, Meta = (Category = "Rush Behavior Event - Boost"))
	virtual void OnBoostStageUp(int32 stage);

	UFUNCTION(BlueprintImplementableEvent, Meta = (Category = "Rush Behavior Event - Boost"))
	virtual void OnBoostEnd();
#pragma endregion


#pragma region DEBUG
	private:
		bool _shouldDrawWallCollisionResults;
		bool _shouldDrawCharacterStats;

		void DrawCharacterStats();

	protected:
		virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

	public:
		UFUNCTION(exec, meta = (FriendlyName = "Particle Rush Console - Collision"))
		void ToggleDrawWallCollisionResults();

		UFUNCTION(exec, meta = (FriendlyName = "Particle Rush Console - Gameplay"))
		void ToggleDrawCharacterStats();
#pragma endregion
};


#pragma region INLINE
#pragma endregion