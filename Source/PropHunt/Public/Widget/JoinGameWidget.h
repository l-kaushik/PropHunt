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
	void SetParentWidget(class UMenuWidget* InParentWidget);

	void AddServerToList(UUserWidget* ServerEntry);
	
private:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	void BindEvents();

	UFUNCTION()
	void OnBackButtonClicked();

private:
	class UMenuWidget* ParentWidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ServerListVBox;
};
