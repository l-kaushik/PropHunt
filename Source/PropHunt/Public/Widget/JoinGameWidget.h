// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UJoinGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddServerToList(UUserWidget* ServerEntry);
	void ClearServerList();
	void DisplaySessionLoadingThrobber();
	void HideSessionLoadingThrobber();
	void DisplayNoSessionFoundMessage();
	void HideNoSessionFoundMessage();
	
private:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindEvents();

	UFUNCTION()
	void OnRefreshButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ServerListVBox;

	// Search Session Loading Box
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* SearchSessionLoadingBox;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* CircularThrobber;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionLoadingText;

	// Session Not Found Error
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* SessionNotFoundBox;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* SessionLoadingSizeBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshButton;
};
