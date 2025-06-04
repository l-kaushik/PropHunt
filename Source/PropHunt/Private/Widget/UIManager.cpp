// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UIManager.h"
#include "Utils/GlobalUtils.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/ErrorBox/UIErrorBox.h"
#include "Widget/MenuWidget.h"
#include "Widget/LobbyWidget.h"
#include "Widget/PlayerEntryWidget.h"
#include "Widget/ServerEntryWidget.h"
#include "Widget/LoadingScreenWidget.h"
#include "Widget/GameStatsWidget.h"
#include "Widget/GameStatsEntryWidget.h"
#include "Widget/TopPerformersWidget.h"
#include "Widget/ScoreboardMenuWidget.h"
#include "Widget/MatchCardWidget.h"
#include "Widget/OptionWidget.h"
#include "Widget/Components/SelectionBox.h"
#include "Widget/PauseMenu.h"

UUIManager* UUIManager::Get()
{
	static UUIManager* Instance = NewObject<UUIManager>();
	return Instance;
}

UUIManager::UUIManager()
{
	InitializeWidgets();
}

void UUIManager::InitializeWidgets()
{
	FString BasePath = "/Game/Widgets/";

	LOAD_WIDGET_BP(HostWidgetBPClassRef, UHostWidget, "WB_Host");
	LOAD_WIDGET_BP(JoinGameWidgetBPClassRef, UJoinGameWidget, "WB_JoinGame");
	LOAD_WIDGET_BP(UIErrorBoxBPClassRef, UUIErrorBox, "ErrorBox/WB_UIErrorBox");
	LOAD_WIDGET_BP(MenuWidgetBPClassRef, UMenuWidget, "WB_Menu");
	LOAD_WIDGET_BP(LobbyWidgetBPClassRef, ULobbyWidget, "WB_Lobby");
	LOAD_WIDGET_BP(PlayerEntryWidgetBPClassRef, UPlayerEntryWidget, "WB_PlayerEntry");
	LOAD_WIDGET_BP(ServerEntryWidgetBPClassRef, UServerEntryWidget, "WB_ServerEntry");
	LOAD_WIDGET_BP(LoadingScreenWidgetBPClassRef, ULoadingScreenWidget, "WB_LoadingScreen");
	LOAD_WIDGET_BP(GameStatsWidgetBPClassRef, UGameStatsWidget, "WB_GameStats");
	LOAD_WIDGET_BP(GameStatsEntryWidgetBPClassRef, UGameStatsEntryWidget, "WB_GameStatsEntry");
	LOAD_WIDGET_BP(TopPerformersWidgetBPClassRef, UTopPerformersWidget, "WB_TopPerformers");
	LOAD_WIDGET_BP(ScoreboardMenuWidgetBPClassRef, UScoreboardMenuWidget, "WB_ScoreboardMenu");
	LOAD_WIDGET_BP(MatchCardWidgetBPClassRef, UMatchCardWidget, "WB_MatchCard");
	LOAD_WIDGET_BP(OptionWidgetBPClassRef, UOptionWidget, "WB_OptionWidget");
	LOAD_WIDGET_BP(SelectionBoxWidgetBPClassRef, USelectionBox, "WB_SelectionBox");
	LOAD_WIDGET_BP(PauseMenuWidgetBPClassRef, UPauseMenu, "WB_PauseMenu");
}

