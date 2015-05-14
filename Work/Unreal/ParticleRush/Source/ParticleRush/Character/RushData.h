// Particle Rush 2014 - 2015

#pragma once

#include "Generic/DataStructs.h"

#include "RushData.generated.h"

/**
* The main data structure for the Hero with all possible data.
* Find legends for Generic Datatypes at the bottom.
*/
USTRUCT(BlueprintType)
struct FRushData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Walk"))
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Walk"))
	float WalkAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Walk"))
	float WalkDeceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Walk"))
	float WalkGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Walk"))
	FRotator WalkRotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Walk"))
	FDataVector2 WalkCameraLagSpeeds;

	
	/* How fast the Mesh should rotate in-order to achieve MeshMaxPitchAngle */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Turning"))
	float MeshTurningSpeed;

	/* The Maximum Roll the Mesh can achieve while turning */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Turning"))
	float MeshTurningMaxAngle;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Boost"))
	float BoostDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Boost"))
	float BoostChainResetDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Boost"))
	int32 MaxBoostStages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Boost"))
	FDataVector2 BoostCameraLagSpeeds;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceJumpFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceOrientationStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	FDataVector2 BounceStrength;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float RefractDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float RefractOrientationStrength;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "SharpTurn"))
	float SharpTurnStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "SharpTurn"))
	FDataVector2 SharpTurnCameraLagSpeeds;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "HardStop"))
	float HardStopDriftDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "HardStop"))
	float HardStopOrientationStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "HardStop"))
	float HardStopBreakingFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "HardStop"))
	float HardStopGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "HardStop"))
	FDataVector2 HardStopCameraLagSpeeds;
};

/* Legend:
* CameraLagSpeeds	- FDataVector2		- Linear Lag Speed, Rotational Lag Speed
* Strengths			- FDataVector2		- Max Strength, Min Strength
*/