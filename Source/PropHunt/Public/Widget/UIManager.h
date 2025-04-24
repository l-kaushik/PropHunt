// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
	// Singleton accesser
	static UUIManager* Get();

	void InitializeWidgets();

	UPROPERTY()
	TSubclassOf<class UHostWidget> HostWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UJoinGameWidget> JoinGameWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UUIErrorBox> UIErrorBoxBPClassRef;

	UPROPERTY()
	TSubclassOf<class UMenuWidget> MenuWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class ULobbyWidget> LobbyWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UPlayerEntryWidget> PlayerEntryWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UServerEntryWidget> ServerEntryWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class ULoadingScreenWidget> LoadingScreenWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UGameStatsWidget> GameStatsWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UGameStatsEntryWidget> GameStatsEntryWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UTopPerformersWidget> TopPerformersWidgetBPClassRef;
private:
	// Private constructor to enforce singleton
	UUIManager();
	
};
