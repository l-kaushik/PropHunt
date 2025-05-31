// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PropCharacter.h"
#include "Characters/PropHuntCharacter.h"
#include "States/PropHuntPlayerState.h"
#include "Actors/SpawnedProp.h"
#include "Controller/PropHuntPlayerController.h"
#include "Interfaces/PropHuntGameModeInterface.h"
#include "GameModes/PropHuntGameMode.h"
#include "Utils/PropHuntLog.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

const float APropCharacter::MAX_HEALTH = 100.0f;


// Sets default values
APropCharacter::APropCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create prop mesh
	PropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMesh"));
	PropMesh->SetupAttachment(GetMesh());

	// enable replication
	SetReplicates(true);
	SetReplicateMovement(true);

	// default values
	Health = MAX_HEALTH;
}

// Called when the game starts or when spawned
void APropCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APropCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APropCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APropHuntPlayerController* PlayerController = Cast<APropHuntPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void APropCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APropCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APropCharacter::Look);

		// Change prop
		EnhancedInputComponent->BindAction(ChangePropAction, ETriggerEvent::Triggered, this, &APropCharacter::ChangePropOnServer);

		// Spawn prop
		EnhancedInputComponent->BindAction(SpawnPropAction, ETriggerEvent::Triggered, this, &APropCharacter::SpawnPropOnServer);

		// Increase camera distance
		EnhancedInputComponent->BindAction(CameraDistanceIncreaseAction, ETriggerEvent::Triggered, this, &APropCharacter::CameraDistanceIncrease);

		// Decrease camera distance
		EnhancedInputComponent->BindAction(CameraDistanceDecreaseAction, ETriggerEvent::Triggered, this, &APropCharacter::CameraDistanceDecrease);

		// Rotate left
		EnhancedInputComponent->BindAction(RotateLeftAction, ETriggerEvent::Triggered, this, &APropCharacter::RotateLeftOnServer);

		// Rotate right
		EnhancedInputComponent->BindAction(RotateRightAction, ETriggerEvent::Triggered, this, &APropCharacter::RotateRightOnServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APropCharacter::CameraDistanceIncrease()
{
	ChangeCameraDistance(50.0f);
}

void APropCharacter::CameraDistanceDecrease()
{
	ChangeCameraDistance(-50.0f);
}

void APropCharacter::ChangeCameraDistance(float Offset)
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + Offset, 0.0f, 3000.0f);
	UE_LOG_NON_SHIP(LogPropCharacter, Warning, TEXT("ArmLength: %f"), CameraBoom->TargetArmLength);
}

void APropCharacter::RotateLeftOnServer_Implementation()
{
	RotatePropMulticast(5.0f);
	UE_LOG_NON_SHIP(LogPropCharacter, Warning, TEXT("Rotated left"));
}

void APropCharacter::RotateRightOnServer_Implementation()
{
	RotatePropMulticast(-5.0f);
	UE_LOG_NON_SHIP(LogPropCharacter, Warning, TEXT("Rotated left"));
}

void APropCharacter::RotatePropMulticast_Implementation(float Offset)
{
	FRotator NewRotation(0.0f, Offset, 0.0f);
	PropMesh->AddLocalRotation(NewRotation);
}

float APropCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority()) {
		Health = FMath::Clamp<double>(Health - DamageAmount, 0.0f, MAX_HEALTH);

		if (IPropHuntControllerInterface* PropHuntInterface = Cast<IPropHuntControllerInterface>(GetController()))
		{
			PropHuntInterface->UpdateHealthWidget(Health);
		}

		// update damage dealt of damage causer and update damage taken of this player
		AddDamageDealtToCauser(DamageCauser, DamageAmount);
		AddDamageTaken(DamageAmount);
		UpdateDamageTracker(DamageCauser, DamageAmount);

		if (Health <= 0.0f)
		{
			// add kill and assist
			AddKillAndAssist();

			// update hidden time
			//UpdatePlayerHiddenTimer();

			if (auto* GameMode = Cast<APropHuntGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->HandlePropDeath(Cast<APropHuntPlayerController>(GetController()));
			}
		}
	}

	return DamageAmount;
}

APropHuntPlayerState* APropCharacter::GetDamageCauserPlayerState(AActor* DamageCauser)
{
	auto* DamageCauserCharacter = Cast<APropHuntCharacter>(DamageCauser);
	if (!DamageCauserCharacter) return nullptr;

	auto* DamageCauserPlayerState = DamageCauserCharacter->GetPlayerState<APropHuntPlayerState>();
	if (!DamageCauserPlayerState) return nullptr;

	return DamageCauserPlayerState;
}

void APropCharacter::AddDamageDealtToCauser(AActor* DamageCauser, float DamageAmount)
{
	if (auto* DamageCauserPlayerState = GetDamageCauserPlayerState(DamageCauser))
	{
		DamageCauserPlayerState->AddDamageDealt(DamageAmount);
	}
}

void APropCharacter::AddDamageTaken(float DamageAmount)
{
	auto* PropPlayerState = GetPlayerState<APropHuntPlayerState>();
	if (!PropPlayerState) return;

	PropPlayerState->AddDamageTaken(DamageAmount);
}

void APropCharacter::UpdateDamageTracker(AActor* DamageCauser, float DamageAmount)
{
	auto* PropPlayerState = GetDamageCauserPlayerState(DamageCauser);
	if (!PropPlayerState) return;

	DamageTracker.FindOrAdd(PropPlayerState) += DamageAmount;
}

void APropCharacter::AddKillAndAssist()
{
	APropHuntPlayerState* TopDamager = nullptr;
	float MaxDamage = -1.0f;

	// get max damage and the top damager
	for (const auto& Pair : DamageTracker)
	{
		if (Pair.Value > MaxDamage)
		{
			MaxDamage = Pair.Value;
			TopDamager = Pair.Key;
		}
	}

	// add kill and assist to players
	for (const auto& Pair : DamageTracker)
	{
		if (Pair.Key == TopDamager)
		{
			Pair.Key->AddKill();
		}
		else
		{
			Pair.Key->AddAssist();
		}
	}
}

void APropCharacter::UpdatePlayerHiddenTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Start Player Hidden Timer called"));

	if (!bHasStartedHiddenTimer) return;

	auto* MyPlayerState = GetPlayerState<APropHuntPlayerState>();

	// Time elapsed since the player became hidden
	FTimespan Elapsed = FDateTime::Now() - CurrentHiddenTime;

	// Convert saved hidden time from PlayerState (assume it's stored in seconds)
	double SavedHiddenTimeSeconds = MyPlayerState->GetHiddenTime();

	// If player has stayed hidden longer, update the state
	if (Elapsed.GetTotalSeconds() > SavedHiddenTimeSeconds)
	{
		MyPlayerState->SetBestHiddenTimer(Elapsed.GetTotalSeconds());
	}

	// clear timer when player move
	GetWorld()->GetTimerManager().ClearTimer(PlayerHiddenTimer);
	bHasStartedHiddenTimer = false;
}

void APropCharacter::StartPlayerHiddenTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Start Player Hidden Timer called"));

	GetWorld()->GetTimerManager().SetTimer(
		PlayerHiddenTimer,
		[this]() {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Start Player Hidden Timer lambda called"));
			CurrentHiddenTime = FDateTime::Now();
			bHasStartedHiddenTimer = true;
		},
		3.f,
		false
	);
}

void APropCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// handle player hidden timer
		// do this on server to solve replication problems but frequent update isn't good.
		//UpdatePlayerHiddenTimer();

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// start hidden timer
		//StartPlayerHiddenTimer();
	}
}

void APropCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


// Prop changing mechanism

void APropCharacter::ChangePropOnServer_Implementation() {
	PerformSphereTrace();
}

void APropCharacter::PerformSphereTrace() {
	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 10.0);
	FVector End = Start;

	// trace parameters	
	float Radius = 100.0;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	bool bTraceComplex = true;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	TArray<FHitResult> OutHits;
	bool bIgnoreSelf = true;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 1.0f;

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(World, Start, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHits, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);

	if (bHit) {
		for (const FHitResult& Hit : OutHits)
		{
			/*
			* let player switch to any prop, except LODGroup = large prop.
			*/
			UStaticMesh* StaticMesh = GetTracedObjectMesh(Hit.GetActor());
			if (StaticMesh && (StaticMesh->GetName().StartsWith(TEXT("PropSwitch_"))))
			{
				UpdateMeshMulticast(StaticMesh);
				break;
			}
		}
	}
}

UStaticMesh* APropCharacter::GetTracedObjectMesh(AActor* HitActor) {
	if (!HitActor) {
		UE_LOG(LogTemp, Error, TEXT("HitActor is not found while changing pro, returning nullptr"));
		return nullptr;
	}

	AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(HitActor);
	if (!MeshActor) {
		UE_LOG(LogTemp, Error, TEXT("StaticMeshActor is not found while changing pro, returning nullptr"));
		return nullptr;
	}

	UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
	if (!MeshComp) {
		UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent is not found while changing pro, returning nullptr"));
		return nullptr;
	}

	UStaticMesh* StaticMesh = MeshComp->GetStaticMesh();
	if (!StaticMesh) {
		UE_LOG(LogTemp, Error, TEXT("StaticMesh is not found while changing prop, returning nullptr"));
		return nullptr;
	}

	return StaticMesh;
}

void APropCharacter::UpdateMeshMulticast_Implementation(UStaticMesh* PropData) {
	PropMesh->SetStaticMesh(PropData);
}

/* Spawn duplicate props */

void APropCharacter::SpawnPropOnServer_Implementation() {

	UWorld* World = GetWorld();

	TSubclassOf<ASpawnedProp> PropClass = ASpawnedProp::StaticClass();
	FTransform SpawnTransform = PropMesh->GetComponentTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASpawnedProp* SpawnedProp = World->SpawnActor<ASpawnedProp>(PropClass, SpawnTransform, SpawnParams);
	UStaticMesh* StaticMesh = PropMesh->GetStaticMesh();

	if (SpawnedProp && StaticMesh) {
		SpawnedProp->SetReplicatedMesh(StaticMesh);
		SpawnedProp->ResetCollision();

		SpawnedPropQueue.Enqueue(SpawnedProp);
		SpawnedPropCount++;
	}

	if (SpawnedPropCount > MAX_DUPLICATE_PROP)
	{
		ASpawnedProp* ReturnedSpawnedProp = nullptr;
		if (SpawnedPropQueue.Dequeue(ReturnedSpawnedProp) && IsValid(ReturnedSpawnedProp))
		{
			ReturnedSpawnedProp->Destroy();
			SpawnedPropCount--;
		}
	}
}