// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/PropHuntControllerInterface.h"
#include "PropHuntPlayerController.generated.h"

/**
 * 
 */

class UMainHud;

UCLASS()
class PROPHUNT_API APropHuntPlayerController : public APlayerController, public IPropHuntControllerInterface
{
	GENERATED_BODY()


public:

	APropHuntPlayerController();

	virtual void BeginPlay() override;

public:
	// PropHuntControllerInterface implementations

	virtual void SetupPropWidget(bool bIsProp) override;

	void TrySetupPropWidget(bool bIsProp);

protected:
	UFUNCTION(Client, Reliable)
	void HandleHudWidgetOnClient(bool bIsProp);

protected:
	// stores the refernce of blueprint WB_MainHud class
	UPROPERTY(EditDefaultsOnly, Category = "Blueprints Reference")
	TSubclassOf<UMainHud> MainHudBPClassRef;

	// stores the object created in BeginPlay
	UMainHud* MainHudRef;
	
};
