// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/PropHuntCharacter.h"
#include "States/PropHuntPlayerState.h"
#include "Controller/PropHuntPlayerController.h"
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
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UParticleSystem* APropHuntCharacter::HitParticle = nullptr;
UAnimationAsset* APropHuntCharacter::FireAnim = nullptr;
UAnimSequence* APropHuntCharacter::WeaponReloadAnim = nullptr;
UAnimMontage* APropHuntCharacter::PlayerReloadMontage = nullptr;

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APropHuntCharacter

APropHuntCharacter::APropHuntCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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

	// Create weapon mesh
	RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
	RifleMesh->SetupAttachment(GetMesh(), TEXT("RifleSocket"));

	// enable replication
	SetReplicates(true);
	SetReplicateMovement(true);

	
	// private variables initialization
	BulletDistance = 10000.0f;

	// load hit particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/MilitaryWeapSilver/FX/P_Impact_Stone_Large_01.P_Impact_Stone_Large_01"));

	if (ParticleAsset.Succeeded()) {
		HitParticle = ParticleAsset.Object;
	}

	// load fire rifle animation
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimationAsset(TEXT("/Game/MilitaryWeapSilver/Weapons/Animations/Fire_Rifle_W.Fire_Rifle_W"));

	if (AnimationAsset.Succeeded()) {
		FireAnim = AnimationAsset.Object;
	}

	// load weapon reload animation sequence
	static ConstructorHelpers::FObjectFinder<UAnimSequence> WeaponReloadAnimationSequence(TEXT("/Game/MilitaryWeapSilver/Weapons/Animations/Reload_Rifle_Hip_W.Reload_Rifle_Hip_W"));

	if (WeaponReloadAnimationSequence.Succeeded()) {
		WeaponReloadAnim = WeaponReloadAnimationSequence.Object;
	}

	// load player reload animation montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerReloadAnimationMontage(TEXT("/Game/MilitaryCharSilver/Animations/Rifle/AM_MM_Rifle_Reload.AM_MM_Rifle_Reload"));

	if (PlayerReloadAnimationMontage.Succeeded()) {
		PlayerReloadMontage = PlayerReloadAnimationMontage.Object;
	}
	else
	{
		UE_LOG(LogPropHuntCharacter, Warning, TEXT("Failed to get anim montage for player reload"))
	}

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APropHuntCharacter::SetCameraSensitivity(float NewSensitivity)
{
	CameraSensitivity = NewSensitivity;
}

//////////////////////////////////////////////////////////////////////////
// Input

void APropHuntCharacter::NotifyControllerChanged()
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

void APropHuntCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APropHuntCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APropHuntCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &APropHuntCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &APropHuntCharacter::StopShooting);

		// Reloading
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APropHuntCharacter::ReloadWeapon);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APropHuntCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
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
	}
}

void APropHuntCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * CameraSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * CameraSensitivity);
	}
}

void APropHuntCharacter::Jump() {
	Super::Jump();
	isJumping = true;
}

void APropHuntCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit); 
	isJumping = false;
}

/*
* Weapon reloading
*/

bool APropHuntCharacter::CanReload()
{
	auto* PS = GetPlayerState<APropHuntPlayerState>();

	int32 Current = PS->GetCurrentAmmoInMagazine();
	int32 Max = PS->GetMaxAmmoInMagazine();
	int32 Reserve = PS->GetCurrentReserveAmmo();

	// Reload only if magazine not full and there's reserve ammo
	return (Current < Max && Reserve > 0);
}

void APropHuntCharacter::ReloadWeapon()
{
	if (IsReloading || !CanReload()) return;

	// stop shooting if shooting
	if (IsShooting) StopShooting();

	IsReloading = true;

	RequestUpdateBulletCount();

	// play montage/sequence
	RequestReloadAnimation();
}

void APropHuntCharacter::RequestUpdateBulletCount_Implementation()
{
	auto* CustomPlayerState = GetPlayerState<APropHuntPlayerState>();

	int32 CurrentAmmoInMagazine = CustomPlayerState->GetCurrentAmmoInMagazine();
	int32 CurrentReserverAmmo = CustomPlayerState->GetCurrentReserveAmmo();

	int32 AmmoNeeded = CustomPlayerState->GetMaxAmmoInMagazine() - CurrentAmmoInMagazine;

	// update weapon count

	if (CurrentReserverAmmo == 0) return;
	
	if (CurrentReserverAmmo >= AmmoNeeded)
	{
		CurrentReserverAmmo -= AmmoNeeded;
		CurrentAmmoInMagazine += AmmoNeeded;
	}
	else
	{
		CurrentAmmoInMagazine += CurrentReserverAmmo;
		CurrentReserverAmmo = 0;
	}
	
	// update player state
	CustomPlayerState->SetCurrentAmmoInMagazine(CurrentAmmoInMagazine);
	CustomPlayerState->SetCurrentReserveAmmo(CurrentReserverAmmo);

	// update UI
	SyncAmmoUI();
}

void APropHuntCharacter::RequestReloadAnimation_Implementation()
{
	MulticastReloadAnimation();
}

void APropHuntCharacter::MulticastReloadAnimation_Implementation()
{
	// play weapon sequence
	// animation playing but not in sync
	//RifleMesh->PlayAnimation(WeaponReloadAnim, false);

	// play player montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageBlendingOut.AddUniqueDynamic(this, &APropHuntCharacter::OnReloadMontageBlendingOut);
		float seconds = AnimInstance->Montage_Play(PlayerReloadMontage);
	}
	else
	{
		UE_LOG(LogPropHuntCharacter, Warning, TEXT("Failed to get anim instance"));
	}
}

void APropHuntCharacter::OnReloadMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	IsReloading = false;
}

void APropHuntCharacter::SyncAmmoUI()
{
	auto* PS = GetPlayerState<APropHuntPlayerState>();

	int32 Magazine = PS->GetCurrentAmmoInMagazine();
	int32 Reserve = PS->GetCurrentReserveAmmo();
	ReceivedUpdateWeaponUI(Magazine, Reserve);
}

void APropHuntCharacter::ReceivedUpdateWeaponUI_Implementation(int32 InCurrentAmmoInMagazine, int32 InCurrentReserverAmmo)
{
	auto* PlayerController = Cast<APropHuntPlayerController>(GetController());
	PlayerController->UpdateWeaponUI(InCurrentAmmoInMagazine, InCurrentReserverAmmo);
}

/*
	Shoot and StopShooting will get called after the "ShootAction" triggers a valid event.

	!HasAuthority make sure only client calls the RPCs.
*/

void APropHuntCharacter::Shoot() {
	if (IsReloading) return;

	UE_LOG_NON_SHIP(LogPropHuntCharacter, Display, TEXT("Started Firing"));
	IsShooting = true;
	FireOnServer();
}

void APropHuntCharacter::StopShooting() {
	UE_LOG_NON_SHIP(LogPropHuntCharacter, Display, TEXT("Stopped Firing"));
	IsShooting = false;
	StopFireOnServer();
}

/*
	Binding the timer to call fire function after every given fire rate.
	Performing on server side
*/

void APropHuntCharacter::FireOnServer_Implementation() {
	float FireRate = 0.1f;
		
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APropHuntCharacter::Fire, FireRate, true);
}

/*
	Clearing the timer to stop firing
*/

void APropHuntCharacter::StopFireOnServer_Implementation() {
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void APropHuntCharacter::Fire() {
	if (isJumping || GetCharacterMovement()->IsFalling() || IsReloading) return;

	// reload automatically if no ammo
	if (GetPlayerState<APropHuntPlayerState>()->GetCurrentAmmoInMagazine() == 0)
	{
		ReloadWeapon();
		return;
	}

	SendCameraRotationForTrace();

	// reduce ammo
	GetPlayerState<APropHuntPlayerState>()->ReduceAmmo();

	// update UI
	SyncAmmoUI();
}

void APropHuntCharacter::SendCameraRotationForTrace_Implementation() {
	FRotator CameraRotation = FollowCamera->GetComponentRotation();
	LineTraceOnServer(CameraRotation);
}

void APropHuntCharacter::LineTraceOnServer_Implementation(FRotator CameraRotation){
	PerformLineTrace(CameraRotation);
	FireMulticast();	
}

void APropHuntCharacter::PerformLineTrace(FRotator CameraRotation) {

	UWorld* World = GetWorld();
	if (!World) return;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (UKismetMathLibrary::GetForwardVector(CameraRotation) * BulletDistance);

	// trace parameters	
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Camera);
	bool bTraceComplex = true;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	FHitResult OutHit;
	bool bIgnoreSelf = true;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = 1.0f;

	// perform the line trace
	bool bHit = UKismetSystemLibrary::LineTraceSingle(World, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);

	// spawn emitter if hit something
	if (bHit) {
		HitFxMulticast(OutHit.ImpactPoint);
		GiveDamage(OutHit.GetActor());
	}
}

void APropHuntCharacter::GiveDamage(AActor* HitActor)
{
	if(!HitActor->ActorHasTag("Prop")) return;

	TSubclassOf<UDamageType> DamageType;
	float Damage = 5.0f;
	UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigator()->GetController(), this, DamageType);

	ShowHitMarker();
}

void APropHuntCharacter::ShowHitMarker()
{
	if (IPropHuntControllerInterface* ControllerInterface = Cast<IPropHuntControllerInterface>(GetController())) {
		ControllerInterface->PlayHitMarkerAnim();
	}
}

void APropHuntCharacter::FireMulticast_Implementation() {
	if (RifleMesh && FireAnim) {
		RifleMesh->PlayAnimation(FireAnim, false);
	}
	else
	{
		UE_LOG(LogPropHuntCharacter, Error, TEXT("Failed to load fire anim or weapon is missing! Check the asset path."));
	}
}

void APropHuntCharacter::HitFxMulticast_Implementation(FVector ImpactPoint) {
	if (HitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, FTransform(ImpactPoint));
	}
	else
	{
		UE_LOG(LogPropHuntCharacter, Error, TEXT("Failed to load particle system! Check the asset path."));
	}
}
