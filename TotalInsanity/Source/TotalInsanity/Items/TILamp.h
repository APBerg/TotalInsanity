// Copyright Adam Berg 2017

#pragma once

#include "CoreMinimal.h"
#include "Items/TIUsableActor.h"
#include "TILamp.generated.h"

/**
 * 
 */
UCLASS()
class TOTALINSANITY_API ATILamp : public ATIUsableActor
{
	GENERATED_BODY()
	
public:
	ATILamp(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, Category = "Lamp Light")
	class UPointLightComponent* PointLight1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Lamp Light")
	float DesiredIntensity;
	
	void OnUsed(APawn * InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_IsOn, Category = "Lamp Light")
	bool bIsOn;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsOn)
	FLinearColor LightColour;

	UFUNCTION()
	void OnRep_IsOn();

	void TurnOn();
};
