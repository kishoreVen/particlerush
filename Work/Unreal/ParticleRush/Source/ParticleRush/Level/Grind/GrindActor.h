// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GrindData.h"
#include "GrindActor.generated.h"

UCLASS()
class PARTICLERUSH_API AGrindActor : public AActor
{
	GENERATED_BODY()

#pragma region Component Declerations
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class USplineComponent* SplineComp;
#pragma endregion

#pragma region Constructor
public:
	// Sets default values for this actor's properties
	AGrindActor();
#pragma endregion
	
#pragma region Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Basic Data"))
	FGrindData GrindData; 
#pragma endregion
public:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;	
};
