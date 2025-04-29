// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIsHost(bool bIsHost);
	void SetParentWidget(class UUserWidget* InParentWidget);
	void AddPlayerToList(UUserWidget* PlayerWidget);
	void ClearPlayerList();

	bool GetIsHost();

	void DisableStartButton();
	void EnableStartButton();

	
private:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindEvents();

	UFUNCTION()
	void OnReadyOrStartButtonClicked();
	UFUNCTION()
	void OnBackButtonClicked();

	void InitializeComponents();
	void InitializeReadyOrStartButtonText();
	void InitializePlayersListHeader();

private:
	// Reference to parent widget
	UPROPERTY()
	class UUserWidget* ParentWidget;

	UPROPERTY(meta = (BindWidget))
	class UMasterButton* ReadyOrStartButton;

	UPROPERTY(meta = (BindWidget))
	class UMasterButton* BackButton;	// close icon in game

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MapNameBlock;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostNameBlock;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* PlayersListVBox;

	UPROPERTY(meta = (BindWidget))
	class UPlayerEntryWidget* PlayersListHeader;

	bool IsHost;
};
