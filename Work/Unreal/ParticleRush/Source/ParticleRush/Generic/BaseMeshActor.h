// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseMeshActor.generated.h"

UCLASS()
class PARTICLERUSH_API ABaseMeshActor : public AActor
{
	GENERATED_BODY()

#pragma region Component Declerations
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* StaticMeshComp;
#pragma endregion

#pragma region Constructor
public:
	// Sets default values for this actor's properties
	ABaseMeshActor();
#pragma endregion

#pragma region Public Interface
public:
	class UStaticMeshComponent* GetStaticMeshComponent();

	void SetStaticMesh(UStaticMesh* NewStaticMesh);

	class UStaticMesh* GetStaticMesh();
#pragma endregion
};
