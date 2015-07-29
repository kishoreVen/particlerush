// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GrindData.h"
#include "GrindActor.generated.h"

UCLASS()
class PARTICLERUSH_API AGrindActor : public AActor
{
	GENERATED_BODY()

#pragma region COMPONENT DECLERATIONS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class USplineComponent* SplineComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (Category = "Curve"))
	class UCurveFloat* VelocityCurve;
#pragma endregion


#pragma region CONSTRUCTOR
public:
	// Sets default values for this actor's properties
	AGrindActor();
#pragma endregion


#pragma region FIELDS
private:
	bool mShouldActivateRamp;

	TWeakObjectPtr<class ARushCharacter> mRushPtr;
#pragma endregion
	

#pragma region EXPOSED
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Basic Data"))
	FGrindData GrindData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Basic Data"))
	float SplineSearchDistanceSqrdThreshold;
#pragma endregion


#pragma region OVERRIDES
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Called every time actors collide
	virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;

	//Called every time actor exits
	virtual void NotifyActorEndOverlap(class AActor* OtherActor) override;
#pragma endregion
};
