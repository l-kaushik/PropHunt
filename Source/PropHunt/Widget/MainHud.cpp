// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextBlock.h"
#include "MainHud.h"

void UMainHud::SetupPropWidget(bool bIsProp)
{
	FText GameStatusText = bIsProp ? FText::FromString("You are the Prop! Hide!") :
		FText::FromString("Find the Prop!");
	GameStatus->SetText(GameStatusText);
}

void UMainHud::NativeConstruct()
{
	if (GameStatus) {
		GameStatus->SetText(FText::FromString("Waiting for players to join..."));
	}
}
