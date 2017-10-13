// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TotalInsanityCharacter.generated.h"

UCLASS()
class TOTALINSANITY_API ATotalInsanityCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	void OnStartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnStartFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	ATotalInsanityCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<class AWeapon> DefaultWapeon;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	virtual void MoveForward(float Val);

	virtual void MoveRight(float Val);

	// Client mapped to Input
	void OnStartJump();

	// Client mapped to Input
	void OnStopJump();

	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;

	// Is character currently performing a jump action. Resets on landed. 
	UPROPERTY(Transient, Replicated)
	bool bIsJumping;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;

	void SetIsJumping(bool NewJumping);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetIsJumping(bool NewJumping);

	void Landed(const FHitResult& Hit) override;

	// Use the usable actor currently in focus, if any
	virtual void Use();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	class ATIUsableActor* GetUsableInView();

	//Max distance to use/focus on actors.
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	// True only in first frame when focused on a new usable actor.
	bool bHasNewFocus;

	class ATIUsableActor* FocusedUsableActor;

	class AWeapon* Weapon;
};
