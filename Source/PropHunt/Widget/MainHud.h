// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameStatus;
	
};
