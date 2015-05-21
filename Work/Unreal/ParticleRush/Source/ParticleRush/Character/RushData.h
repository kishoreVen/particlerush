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

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float BounceOrientationStrength;

	UPROPERTY(EditAnywhere, Meta = (Category = "Bounce"))
	FStrengthData BounceStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	FStrengthData BounceJumpStrength;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float RefractDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Bounce"))
	float RefractOrientationStrength;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "SharpTurn"))
	float SharpTurnStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "SharpTurn"))
	FStrengthData SharpTurnYawReach;


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


	void UpdateProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
	{
		UProperty* field = PropertyChangedEvent.PropertyChain.GetTail()->GetPrevNode()->GetValue();
		FName fieldName = (field != NULL) ? field->GetFName() : NAME_None;

		if (fieldName == "BounceStrength")
			BounceStrength.UpdateProperties();
		else if (fieldName == "BounceJumpStrength")
			BounceJumpStrength.UpdateProperties();
		else if (fieldName == "SharpTurnYawReach")
			SharpTurnYawReach.UpdateProperties();
	}
};

/* Legend:
* CameraLagSpeeds	- FDataVector2		- Linear Lag Speed, Rotational Lag Speed
* Strengths			- FDataVector2		- Max Strength, Min Strength
*/