// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/Button/MasterButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"



void UMasterButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button && !bEventBound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bind event for master button called"));
		Button->OnClicked.AddDynamic(this, &UMasterButton::HandleButtonClicked);
		bEventBound = true;
	}
}

void UMasterButton::HandleButtonClicked()
{
	OnClicked.Broadcast();
}
