// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextBlock.h"
#include "MainHud.h"

void UMainHud::NativeConstruct()
{
	if (GameStatus) {
		GameStatus->SetText(FText::FromString("Waiting for players to join..."));
	}
}
