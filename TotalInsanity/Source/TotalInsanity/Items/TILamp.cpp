// Copyright Adam Berg 2017

#include "TILamp.h"
#include "Classes/Components/PointLightComponent.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ATILamp::ATILamp(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bIsOn = true;
	DesiredIntensity = 3000.0f;
	PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
	PointLight1->Intensity = DesiredIntensity; 
	PointLight1->SetVisibility(bIsOn);
}

void ATILamp::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	bIsOn = !bIsOn;
	LightColour = FLinearColor(FMath::FRandRange(0.0f, 1.0f), FMath::FRandRange(0.0f, 1.0f), FMath::FRandRange(0.0f, 1.0f), 1.0f);
	TurnOn();
}

void ATILamp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATILamp, bIsOn);
	DOREPLIFETIME(ATILamp, LightColour);
}

void ATILamp::OnRep_IsOn()
{
	TurnOn();
}

void ATILamp::TurnOn()
{
	if (!bIsOn)
	{
		PointLight1->SetVisibility(false);
	}
	else
	{
		PointLight1->SetLightColor(LightColour);
		PointLight1->SetVisibility(true);
	}
}
