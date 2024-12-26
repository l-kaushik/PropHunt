// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/Button/MasterButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"



void UMasterButton::NativeConstruct()
{
	BindEvent();
}

void UMasterButton::NativePreConstruct()
{
	
}

void UMasterButton::BindEvent()
{
	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UMasterButton::HandleButtonClicked);
	}
}

void UMasterButton::HandleButtonClicked()
{
	OnClicked.Broadcast();
}
