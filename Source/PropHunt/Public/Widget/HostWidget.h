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
class UImage;
class UButton;
class UTextBlock;
struct FMapInfo;

UCLASS()
class PROPHUNT_API UHostWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHostWidget(const FObjectInitializer& ObjectInitializer);

private:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindEvents();

	void InitializeHostButton();

	UFUNCTION()
	void OnHostButtonClicked();

	UFUNCTION()
	void OnLeftImageSelectorClicked();

	UFUNCTION()
	void OnRightImageSelectorClicked();

	void UpdateMapData();
	bool VerifyServerInfo();
	void ShowServerNameError(const FString& InServerName);
	void ShowPlayerNumberError(const FString& InPlayerNumber);

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


	// map selector
	UPROPERTY(meta = (BindWidget))
	UImage* MapImage;

	UPROPERTY(meta = (BindWidget))
	UButton* LeftImageSelector;

	UPROPERTY(meta = (BindWidget))
	UButton* RightImageSelector;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapName;

	int m_MapSelected;
	TArray<FMapInfo> m_MapInfoArray;
	AMenuController* MenuController;
	TSubclassOf<UUIErrorBox> UIErrorBoxBPClassRef;
};
