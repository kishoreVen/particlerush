// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class PARTICLERUSH_API AObstacle : public AActor
{
	GENERATED_BODY()

#pragma region Component Declerations
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* staticMeshComp;
#pragma endregion

#pragma region Component Declerations
protected:
	virtual void InitializeObstacle();
#pragma endregion
	
public:	
	// Sets default values for this actor's properties
	AObstacle(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
