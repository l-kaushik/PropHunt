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

public:
	void SetParentWidget(class UMenuWidget* InParentWidget);

private:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

private:

	UPROPERTY()
	class UMenuWidget* ParentWidget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerNameLabel;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	
};
