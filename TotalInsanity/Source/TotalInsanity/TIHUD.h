// Copyright Adam Berg 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "TIHUD.generated.h"


UCLASS()
class TOTALINSANITY_API ATIHUD : public AHUD
{
	GENERATED_BODY()
	
	ATIHUD(const FObjectInitializer& ObjectInitializer);

	FCanvasIcon CenterDotIcon;

	virtual void DrawHUD() override;

	void DrawCenterDot();

	void DrawInfo();
};
