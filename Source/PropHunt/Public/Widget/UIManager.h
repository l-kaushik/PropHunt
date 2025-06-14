// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

#define LOAD_WIDGET_BP(WidgetClassRef, WidgetType, WidgetName) \
    WidgetClassRef = GlobalUtils::LoadBlueprint<WidgetType>(BasePath + FString(WidgetName));

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

	UPROPERTY()
	TSubclassOf<class UScoreboardMenuWidget> ScoreboardMenuWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UMatchCardWidget> MatchCardWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UOptionWidget> OptionWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class USelectionBox> SelectionBoxWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UPauseMenu> PauseMenuWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UHelpWidget> HelpWidgetBPClassRef;

private:
	// Private constructor to enforce singleton
	UUIManager();
	
};
