// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TLaser.generated.h"

UCLASS()
class PARTICLERUSH_API ATLaser : public AActor
{
	GENERATED_BODY()

#pragma region COMPONENT DECLERATIONS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* StaticMeshBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Category = "Mesh"))
	class UStaticMeshComponent* StaticMeshNozzle;


#pragma endregion
	
#pragma region CONSTRUCTOR
public:
	// Sets default values for this actor's properties
	ATLaser();
#pragma endregion


#pragma region FIELDS
private:
	TWeakObjectPtr<class ARushCharacter> mRushPtr;
#pragma endregion
	

#pragma region EXPOSED
protected:
#pragma endregion

#pragma region OVERRIDES
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//Called every time actors collide
	//virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;

	//Called every time actor exits
	//virtual void NotifyActorEndOverlap(class AActor* OtherActor) override;
#pragma endregion
};
