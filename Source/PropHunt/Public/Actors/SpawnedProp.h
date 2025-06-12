// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnedProp.generated.h"

class APropCharacter;

UCLASS()
class PROPHUNT_API ASpawnedProp : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(Replicated, VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* PropMesh;
	
public:	
	// Sets default values for this actor's properties
	ASpawnedProp();

	UStaticMeshComponent* GetPropMesh() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetReplicatedMesh(UStaticMesh* NewMesh);
	void ResetCollision(float DelayInSeconds = 5.0f);
	void Remove();

private:
	FTimerHandle TimerHandler;
};
