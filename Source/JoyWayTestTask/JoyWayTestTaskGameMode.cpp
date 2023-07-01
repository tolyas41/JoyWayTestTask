// Copyright Epic Games, Inc. All Rights Reserved.

#include "JoyWayTestTaskGameMode.h"
#include "JoyWayTestTaskHUD.h"
#include "JoyWayTestTaskCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJoyWayTestTaskGameMode::AJoyWayTestTaskGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AJoyWayTestTaskHUD::StaticClass();
}
