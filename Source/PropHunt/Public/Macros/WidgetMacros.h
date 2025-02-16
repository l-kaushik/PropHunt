// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*
* Macro for binding a c++ function to any UWidget button type
* If define a custom button type make sure to have the same delegate name "OnClicked"
*/
#define BIND_BUTTON_CLICK(Button, Function)\
if(Button)\
{\
	Button->OnClicked.AddUObject(this, Function);\
}