// Copyright Adam Berg 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TIUsableActor.generated.h"

UCLASS(ABSTRACT)
class TOTALINSANITY_API ATIUsableActor : public AActor
{
	GENERATED_BODY()
	

public:

	virtual void OnBeginFocus();

	virtual void OnEndFocus();

	virtual void OnUsed(APawn* InstigatorPawn);

	ATIUsableActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
