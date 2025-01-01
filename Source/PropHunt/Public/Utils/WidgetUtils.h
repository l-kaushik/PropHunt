// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"

/**
 * 
 */
class PROPHUNT_API WidgetUtils
{
public:
	WidgetUtils();
	~WidgetUtils();

public:
	// Widget creation methods
	template<typename WidgetType, typename OwnerType = UObject>
	static WidgetType* CreateAndValidateWidget(OwnerType* Owner, TSubclassOf<UUserWidget> WidgetBPClassRef = WidgetType::StaticClass())
	{
		if (!WidgetBPClassRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Widget blueprint class reference is invalid"));
			return nullptr;
		}

		WidgetType* WidgetRef = CreateWidget<WidgetType>(Owner, WidgetBPClassRef);

		if (!WidgetRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create widget, check template argument type and widget blueprint class reference"));
			return nullptr;
		}

		return WidgetRef;
	}

	template<typename WidgetType = UUserWidget, typename OwnerType = UObject>
	static WidgetType* CreateAndAddWidget(OwnerType* Owner, UClass* WidgetBPClassRef)
	{
		WidgetType* WidgetRef = CreateAndValidateWidget<WidgetType>(Owner, WidgetBPClassRef);
		if (!WidgetRef) return nullptr;
		WidgetRef->AddToViewport();
		return WidgetRef;
	}

	template<typename WidgetType = UUserWidget, typename ParentWidgetType = UUserWidget, typename OwnerType = UObject>
	static WidgetType* CreateSubWidgetAndHideParent(OwnerType* Owner, UClass* WidgetBPClassRef, ParentWidgetType* ParentWidget)
	{
		WidgetType* WidgetRef = CreateAndAddWidget<WidgetType>(Owner, WidgetBPClassRef);

		if (!WidgetRef) return nullptr;

		WidgetRef->SetParentWidget(ParentWidget);
		ParentWidget->SetVisibility(ESlateVisibility::Collapsed);
		return WidgetRef;
	}

	template<typename WidgetType, typename WidgetSwitcherT = UWidgetSwitcher, typename OwnerType = UObject>
	static void AddWidgetToWidgetSwitcher(OwnerType* Owner, WidgetSwitcherT* WidgetSwitcher, TSubclassOf<UUserWidget> WidgetBPClassRef = WidgetType::StaticClass())
	{
		if (!WidgetBPClassRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetSwitcher object is invalid or null."));
			return;
		}

		if (!WidgetBPClassRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Widget blueprint class reference is invalid"));
			return;
		}

		WidgetSwitcher->AddChild(CreateAndValidateWidget<WidgetType>(Owner, WidgetBPClassRef));
	}


	// Properties setter templated methods
	template <typename ContainerType = UPanelWidget, typename ChildType = UUserWidget>
	static void InitializeContainerBox(ContainerType* Box, ChildType* ChildWidget)
	{
		if (Box && ChildWidget)
		{
			Box->AddChild(ChildWidget);
		}
	}

	template <typename ComponentType, typename SlotType>
	static void SetPaddingAndSize(ComponentType* Component, FMargin margin, FSlateChildSize ChildSize)
	{
		SlotType* ComponentSlot = Cast<SlotType>(Component->Slot);
		if (ComponentSlot)
		{
			ComponentSlot->SetPadding(margin);
			ComponentSlot->SetSize(ChildSize);
		}
	}
};
