// Fill out your copyright notice in the Description page of Project Settings.

#include "TotalInsanityGameMode.h"
#include "TotalInsanityGameState.h"
#include "TotalInsanity/Player/TotalInsanityCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TIHUD.h"


ATotalInsanityGameMode::ATotalInsanityGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/AnimStarterPack/Ue4ASP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	/* Assign the class types used by this gamemode */
	HUDClass = ATIHUD::StaticClass();
	//PlayerControllerClass = ASPlayerController::StaticClass();
	//PlayerStateClass = ASPlayerState::StaticClass();
	//GameStateClass = ASGameState::StaticClass();
	//SpectatorClass = ASSpectatorPawn::StaticClass();



}

