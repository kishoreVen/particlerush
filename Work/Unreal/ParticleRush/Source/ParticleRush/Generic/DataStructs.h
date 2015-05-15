// Particle Rush 2014 - 2015

#pragma once

#include "ParticleRush.h"

#include "DataStructs.generated.h"

/**
* A 2 float datatype
*/
USTRUCT(BlueprintType)
struct FDataVector2
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float DataValue1;
	UPROPERTY(EditAnywhere)
	float DataValue2;

	FDataVector2()
	{
		DataValue1 = 0.0f;
		DataValue2 = 0.0f;
	}

	FDataVector2(float dataValue1, float dataValue2)
	{
		DataValue1 = dataValue1;
		DataValue2 = dataValue2;
	}
};


/**
 * A 3 float datatype
 */
USTRUCT(BlueprintType)
struct FDataVector3
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float DataValue1;
	UPROPERTY(EditAnywhere)
	float DataValue2;
	UPROPERTY(EditAnywhere)
	float DataValue3;

	FDataVector3()
	{
		DataValue1 = 0.0f;
		DataValue2 = 0.0f;
		DataValue3 = 0.0f;
	}
	
	FDataVector3(float dataValue1, float dataValue2, float dataValue3)
	{
		DataValue1 = dataValue1;
		DataValue2 = dataValue2;
		DataValue3 = dataValue3;
	}
};


/**
* A 3 int datatype
*/
USTRUCT(BlueprintType)
struct FCountVector3
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 DataValue1;
	UPROPERTY(EditAnywhere)
	int32 DataValue2;
	UPROPERTY(EditAnywhere)
	int32 DataValue3;

	FCountVector3()
	{
		DataValue1 = 0;
		DataValue2 = 0;
		DataValue3 = 0;
	}

	FCountVector3(int32 dataValue1, int32 dataValue2, int32 dataValue3)
	{
		DataValue1 = dataValue1;
		DataValue2 = dataValue2;
		DataValue3 = dataValue3;
	}
};


/**
* A 4 float datatype
*/
USTRUCT(BlueprintType)
struct FDataVector4
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float DataValue1;
	UPROPERTY(EditAnywhere)
	float DataValue2;
	UPROPERTY(EditAnywhere)
	float DataValue3;
	UPROPERTY(EditAnywhere)
	float DataValue4;

	FDataVector4()
	{
		DataValue1 = 0.0f;
		DataValue2 = 0.0f;
		DataValue3 = 0.0f;
		DataValue4 = 0.0f;
	}

	FDataVector4(float dataValue1, float dataValue2, float dataValue3, float dataValue4)
	{
		DataValue1 = dataValue1;
		DataValue2 = dataValue2;
		DataValue3 = dataValue3;
		DataValue4 = dataValue4;
	}
};


/**
* 1 float and 1 rotator datatype mostly used for Camera
*/
USTRUCT(BlueprintType)
struct FCameraDataVector
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float TargetArmLength;
	UPROPERTY(EditAnywhere)
	FRotator TargetArmRotation;
	UPROPERTY(EditAnywhere)
	FRotator CameraRotation;
	
	FCameraDataVector()
	{
		TargetArmLength = 0.0f;
		TargetArmRotation = FRotator(0.0f, 0.0f, 0.0f);
		CameraRotation = FRotator(0.0f, 0.0f, 0.0f);
	}

	FCameraDataVector(float targetArmLength, FRotator targetArmRotation, FRotator cameraRotation)
	{
		TargetArmLength = targetArmLength;
		TargetArmRotation = targetArmRotation;
		CameraRotation = cameraRotation;
	}
};