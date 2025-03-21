// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PropHuntCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APropHuntCharacter : public ACharacter
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

	/** Weapon Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** Weapon Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RifleMesh;

public:
	APropHuntCharacter();
	

protected:

	virtual void Jump() override;

	virtual void Landed(const FHitResult& Hit) override;

	void Shoot();
	void StopShooting();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


public:
	UFUNCTION(Server, Reliable)
	void FireOnServer();
	UFUNCTION(Server, Reliable)
	void StopFireOnServer();

	UFUNCTION(Client, Reliable)
	void SendCameraRotationForTrace();

	UFUNCTION(Server, Reliable)
	void LineTraceOnServer(FRotator CameraRotation);

	UFUNCTION(NetMulticast, Unreliable)
	void FireMulticast();

	UFUNCTION(NetMulticast, Unreliable)
	void HitFxMulticast(FVector ImpactPoint);

protected:
	UFUNCTION()
	void Fire();
	void GiveDamage(AActor* HitActor);
	void ShowHitMarker();

	void PerformLineTrace(FRotator CameraRotation);


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isJumping;

private:

	static UParticleSystem* HitParticle;
	static UAnimationAsset* FireAnim;

	FTimerHandle TimerHandle;
	float BulletDistance;
};

