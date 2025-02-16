// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostWidget.generated.h"

/**
 * 
 */

class UVerticalBox;
class UEditableText;
class UMasterButton;
class AMenuController;
class UUIErrorBox;

UCLASS()
class PROPHUNT_API UHostWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

private:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindEvents();

	void InitializeServerInfoVBox();
	void InitializeHostButton();

	UFUNCTION()
	void OnHostButtonClicked();

	bool VerifyServerInfo();

private:
	// Components related to server info box
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ServerInfoVBox;

	UPROPERTY(meta = (BindWidget))
	UEditableText* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	UEditableText* NumberOfPlayersText;

	// add combo box for map selection

	UPROPERTY(meta = (BindWidget))
	UMasterButton* HostButton;
	
	AMenuController* MenuController;

	TSubclassOf<UUIErrorBox> UIErrorBoxBPClassRef;
};
