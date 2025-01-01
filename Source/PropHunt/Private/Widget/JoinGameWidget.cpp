// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/JoinGameWidget.h"
#include "Widget/MenuWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"

void UJoinGameWidget::AddServerToList(UUserWidget* ServerEntry)
{
	ServerListVBox->AddChild(ServerEntry);
}

void UJoinGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();
}

void UJoinGameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UJoinGameWidget::BindEvents()
{

}
