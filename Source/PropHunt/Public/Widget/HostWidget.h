// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UHostWidget : public UUserWidget
{
	GENERATED_BODY()
private:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindEvents();

	void InitializeServerInfoVBox();

	UFUNCTION()
	void OnHostButtonClicked();

	bool VerifyServerInfo();

private:
	// Components related to server info box
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ServerInfoVBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* NumberOfPlayersText;

	// add combo box for map selection

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	class AMenuController* MenuController;
};
