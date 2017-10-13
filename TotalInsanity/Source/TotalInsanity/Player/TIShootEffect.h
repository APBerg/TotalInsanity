// Copyright Adam Berg 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TIShootEffect.generated.h"

UCLASS()
class TOTALINSANITY_API ATIShootEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATIShootEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
