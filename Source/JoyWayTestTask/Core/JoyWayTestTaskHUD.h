// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "JoyWayTestTaskHUD.generated.h"

UCLASS()
class AJoyWayTestTaskHUD : public AHUD
{
	GENERATED_BODY()

public:
	AJoyWayTestTaskHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

