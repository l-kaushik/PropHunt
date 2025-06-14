// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/PropHuntControllerInterface.h"
#include "PropHuntPlayerController.generated.h"

/**
 * 
 */

class UMainHud;
class UPauseMenu;
struct FPlayerData;

UCLASS()
class PROPHUNT_API APropHuntPlayerController : public APlayerController, public IPropHuntControllerInterface
{
	GENERATED_BODY()


public:

	APropHuntPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	// PropHuntControllerInterface implementations

	virtual void SetupPropWidget(bool bIsProp) override;
	virtual void UpdateHealthWidget(float NewHealth) override;
	virtual void ShowWinScreenWidget() override;
	virtual void StartCountdownWidget() override;
	virtual void PlayHitMarkerAnim() override;

public:

	void TrySetupPropWidget(bool bIsProp);
	void SetIsProp(bool bIsProp);
	bool GetIsProp();
	void UpdateWeaponUI(int32 InCurrentAmmoInMagazine, int32 InCurrentReserverAmmo);
	bool GetIsHost();

	void StartNewGame();
	void ExitGame();
	void ExitClient();

	void SetCameraSensitivity(float NewSensitivity);

	void TogglePauseMenu();

protected:
	void ShowPauseMenu();
	void HidePauseMenu();

public:
	UFUNCTION(Client, Reliable)
	void ShowLoadingScreen(const FString& InMessage);

protected:
	UFUNCTION(Client, Reliable)
	void HandleHudWidgetOnClient(bool bIsProp);

	UFUNCTION(Client, Reliable)
	void UpdateHealthOnClient(float NewHealth);

	UFUNCTION(Client, Reliable)
	void ShowWinScreenOnClient();

	UFUNCTION(Client, Reliable)
	void StartCountdownOnClient();

	UFUNCTION(Client, Reliable)
	void ShowHitMarkerOnClient();

	UFUNCTION(Server, Reliable)
	void ExitClientOnServer();

	UFUNCTION(Server, Reliable)
	void ServerInitiateLoadingScreen();

	UFUNCTION(Server, Reliable)
	void ServerSendProfileData(const FPlayerData& InPlayerData);

protected:
	// stores the refernce of blueprint WB_MainHud class
	UPROPERTY(EditDefaultsOnly, Category = "Blueprints Reference")
	TSubclassOf<UMainHud> MainHudBPClassRef;

	// stores the object created in BeginPlay
	UMainHud* MainHudRef;

private:
	bool m_bIsProp;
	class UPropHuntGameInstance* PropHuntGameInstance;

	TSubclassOf<class ULoadingScreenWidget> LoadingScreenWidgetBPClassRef;
	class ULoadingScreenWidget* LoadingWidget;

	UPauseMenu* PauseMenuWidget;
	bool bIsPauseMenuVisible = false;
	bool bIsGameEnded = false;
};
