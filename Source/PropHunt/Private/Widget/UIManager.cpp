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

	HostWidgetBPClassRef = GlobalUtils::LoadBlueprint<UHostWidget>(BasePath + FString("WB_Host"));
	JoinGameWidgetBPClassRef = GlobalUtils::LoadBlueprint<UJoinGameWidget>(BasePath + FString("WB_JoinGame"));
	UIErrorBoxBPClassRef = GlobalUtils::LoadBlueprint<UUIErrorBox>(BasePath + FString("ErrorBox/WB_UIErrorBox"));
	MenuWidgetBPClassRef = GlobalUtils::LoadBlueprint<UMenuWidget>(BasePath + FString("WB_Menu"));
	LobbyWidgetBPClassRef = GlobalUtils::LoadBlueprint<ULobbyWidget>(BasePath + FString("WB_Lobby"));
	PlayerEntryWidgetBPClassRef = GlobalUtils::LoadBlueprint<UPlayerEntryWidget>(BasePath + FString("WB_PlayerEntry"));
	ServerEntryWidgetBPClassRef = GlobalUtils::LoadBlueprint<UServerEntryWidget>(BasePath + FString("WB_ServerEntry"));
}

