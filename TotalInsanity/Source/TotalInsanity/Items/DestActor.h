// Copyright Adam Berg 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestActor.generated.h"

UCLASS()
class TOTALINSANITY_API ADestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestActor(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UMaterialInstanceDynamic* DynamicMAT;

	UFUNCTION(Reliable, NetMulticast)
	void SimulateExplosionFX();

	void SimulateExplosionFX_Implementation();

	UPROPERTY(EditAnywhere, Category = "Explosion")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	float Strength;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	UParticleSystem* ExplosionFX;

	class USphereComponent* SphereComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
	UFUNCTION()
	void OnRep_Health();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
