// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UIManager.h"
#include "Utils/GlobalUtils.h"
#include "Widget/HostWidget.h"
#include "Widget/JoinGameWidget.h"
#include "Widget/ErrorBox/UIErrorBox.h"

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

	HostWidgetBPClassRef = GlobalUtils::LoadWidgetBlueprint<UHostWidget>(BasePath + FString("WB_Host"));
	JoinGameWidgetBPClassRef = GlobalUtils::LoadWidgetBlueprint<UJoinGameWidget>(BasePath + FString("WB_JoinGame"));
	UIErrorBoxBPClassRef = GlobalUtils::LoadWidgetBlueprint<UUIErrorBox>(BasePath + FString("ErrorBox/WB_UIErrorBox"));
}

