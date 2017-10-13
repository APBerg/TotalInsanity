// Copyright Adam Berg 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserBeam.h"
#include "Weapon.generated.h"

UCLASS()
class TOTALINSANITY_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserBeam> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USceneComponent* MuzzleLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	void OnFire();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void StartFire();

};
