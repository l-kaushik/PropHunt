// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnedProp.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASpawnedProp::ASpawnedProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMesh"));
	RootComponent = PropMesh;
	
	// enable replication
	bReplicates = true;
	bAlwaysRelevant = true;
	PropMesh->SetIsReplicated(true);

	// default collision settings
	PropMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PropMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

UStaticMeshComponent* ASpawnedProp::GetPropMesh() const
{
	return PropMesh;
}

void ASpawnedProp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpawnedProp, PropMesh);
}

// Called when the game starts or when spawned
void ASpawnedProp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnedProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnedProp::SetReplicatedMesh(UStaticMesh* NewMesh)
{
	PropMesh->SetStaticMesh(NewMesh);
}

void ASpawnedProp::ResetCollision(float DelayInSeconds) {
	UE_LOG(LogTemp, Warning, TEXT("Waiting for 5 seconds"));
	FTimerHandle TimerHandler;
	GetWorldTimerManager().SetTimer(
		TimerHandler,
		[this]() {
			UE_LOG(LogTemp, Warning, TEXT("Collision reset"));
			PropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			PropMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		},
		DelayInSeconds,
		false);
}
