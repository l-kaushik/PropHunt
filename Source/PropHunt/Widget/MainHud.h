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

public:
	void SetupPropWidget(bool bIsProp);
	void UpdateHealthBar(float NewHealth);

protected:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

private:
	void SetGameStatusText();
	void SetCrosshairImage();
	void SetHealthBar();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameStatus;

	UPROPERTY(meta = (BindWidget))
	class UImage* Crosshair;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
};
