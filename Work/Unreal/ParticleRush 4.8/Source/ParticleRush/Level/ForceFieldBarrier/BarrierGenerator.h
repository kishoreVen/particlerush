// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BarrierGenerator.generated.h"

UCLASS()
class PARTICLERUSH_API ABarrierGenerator : public AActor
{
	GENERATED_BODY()
	
#pragma region CONSTRUCTOR
	// Sets default values for this actor's properties
	ABarrierGenerator();
#pragma endregion

#pragma region COMPONENTS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Root"))
	class USceneComponent* BarrierBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Root"))
	class UStaticMeshComponent* BarrierStart;
#pragma endregion

#pragma region OVERRIDES
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// When the property is changed
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;

public:
	UFUNCTION(BlueprintImplementableEvent, Meta = (BlueprintInternalUseOnly = "true", Category = "Barrier Build", DisplayName = "Add Barrier Element"))
	void AddBarrierElement_Test();
#pragma endregion

#pragma region PROPERTIES
protected:
	/* The mesh that will be used as track for this spline */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Barrier Build"))
	class UStaticMesh* MainBarrierMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Barrier Build", MakeEditWidget = ""))
	FVector EndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Category = "Barrier Build", MakeEditWidget = ""))
	TArray<FVector> HeightPoints;
#pragma endregion

#pragma region PROPERTIY ACCESSORS
#pragma endregion

#pragma region FIELDS
private:
#pragma endregion
};
