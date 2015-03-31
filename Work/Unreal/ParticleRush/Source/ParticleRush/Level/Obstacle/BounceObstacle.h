// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BounceObstacle.generated.h"

UCLASS()
class PARTICLERUSH_API ABounceObstacle : public AActor
{
	GENERATED_BODY()

#pragma region Component Declerations
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* staticMeshComp;
#pragma endregion
	
public:	
	// Sets default values for this actor's properties
	ABounceObstacle(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
