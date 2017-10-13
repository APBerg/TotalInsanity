// Copyright Adam Berg 2017

#include "TIHUD.h"
#include "Player/TotalInsanityCharacter.h"
#include "Items/TIUsableActor.h"
#include "GameFramework/PlayerState.h"
#include "Engine.h"
#include "UObject/ConstructorHelpers.h"

ATIHUD::ATIHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* You can use the FObjectFinder in C++ to reference content directly in code. Although it's advisable to avoid this and instead assign content through Blueprint child classes. */
	static ConstructorHelpers::FObjectFinder<UTexture> HUDCenterDotObj(TEXT("/Game/UI/HUD/T_CenterDot_M.T_CenterDot_M"));
	CenterDotIcon = UCanvas::MakeIcon(HUDCenterDotObj.Object);
}


void ATIHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCenterDot();

	DrawInfo();
	

}


void ATIHUD::DrawCenterDot()
{
	float CenterX = Canvas->ClipX / 2;
	float CenterY = Canvas->ClipY / 2;
	float CenterDotScale = 0.07f;

	ATotalInsanityCharacter* Pawn = Cast<ATotalInsanityCharacter>(GetOwningPawn());
	if (Pawn)
	{
		// Boost size when hovering over a usable object.
		ATIUsableActor* Usable = Pawn->GetUsableInView();
		if (Usable)
		{
			CenterDotScale *= 1.5f;
		}

		Canvas->SetDrawColor(255, 255, 255, 255);
		Canvas->DrawIcon(CenterDotIcon,
		CenterX - CenterDotIcon.UL*CenterDotScale / 2.0f,
		CenterY - CenterDotIcon.VL*CenterDotScale / 2.0f, CenterDotScale);
		

	}
}

void ATIHUD::DrawInfo()
{
	ATotalInsanityCharacter* Pawn = Cast<ATotalInsanityCharacter>(GetOwningPawn());
	if (Pawn)
	{
		APlayerState* CurrentState = Pawn->PlayerState;
		if (CurrentState != nullptr)
		{
			
			
			GEngine->AddOnScreenDebugMessage(21, 1.0f, FColor::Red, TEXT("Player ") + FString::FromInt(CurrentState->PlayerId));
			GEngine->AddOnScreenDebugMessage(22, 1.0f, FColor::Red, TEXT("Score: ") + FString::SanitizeFloat(CurrentState->Score));

		}
	}
}
