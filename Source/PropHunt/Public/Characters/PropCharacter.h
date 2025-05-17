// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PropCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UStaticMeshComponent;
class ASpawnedProp;
class APropHuntPlayerState;

UCLASS()
class PROPHUNT_API APropCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Change prop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangePropAction;

	/** Spawn prop input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SpawnPropAction;

	/* Change camera distance input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraDistanceIncreaseAction;

	/* Change camera distance input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraDistanceDecreaseAction;

	/* Rotate Left */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateLeftAction;

	/* Rotate Right */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateRightAction;

	/** Prop Mesh actual player */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PropMesh;

public:
	// Sets default values for this character's properties
	APropCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	/* Change prop */
	UFUNCTION(Server, Reliable)
	void ChangePropOnServer();
	void PerformSphereTrace();
	UStaticMesh* GetTracedObjectMesh(AActor* HitActor);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateMeshMulticast(UStaticMesh* PropData);

	/* Spawn duplicate prop */
	UFUNCTION(Server, Reliable)
	void SpawnPropOnServer();

	void CameraDistanceIncrease();
	void CameraDistanceDecrease();
	void ChangeCameraDistance(float Offset);

	UFUNCTION(Server, Unreliable)
	void RotateLeftOnServer();

	UFUNCTION(Server, Unreliable)
	void RotateRightOnServer();

	UFUNCTION(NetMulticast, Unreliable)
	void RotatePropMulticast(float Offset);

private:
	APropHuntPlayerState* GetDamageCauserPlayerState(AActor* DamageCauser);
	void AddDamageDealtToCauser(AActor* DamageCauser, float DamageAmount);
	void AddDamageTaken(float DamageAmount);
	void UpdateDamageTracker(AActor* DamageCauser, float DamageAmount);
	void AddKillAndAssist();
	void UpdatePlayerHiddenTimer();
	void StartPlayerHiddenTimer();

private:
	float Health;
	static const float MAX_HEALTH;
	TMap<APropHuntPlayerState*, float> DamageTracker;
	FTimerHandle PlayerHiddenTimer;
	FDateTime CurrentHiddenTime;
	bool bHasStartedHiddenTimer = false;
};
