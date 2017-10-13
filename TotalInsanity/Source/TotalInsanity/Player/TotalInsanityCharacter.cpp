// Fill out your copyright notice in the Description page of Project Settings.


#include "TotalInsanityCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/TIUsableActor.h"
#include "Player/Weapon.h"
#include "Net/UnrealNetwork.h"
//#include "DrawDebugHelpers.h"

// Sets default values
ATotalInsanityCharacter::ATotalInsanityCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	MoveComp->GravityScale = 1.5f;
	MoveComp->JumpZVelocity = 620;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->MaxWalkSpeedCrouched = 200;

	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(0, 35, 0);
	CameraBoom->TargetOffset = FVector(0, 0, 55);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	MaxUseDistance = 800;
	bHasNewFocus = true;

}

// Called when the game starts or when spawned
void ATotalInsanityCharacter::BeginPlay()
{
	
	Super::BeginPlay();
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;
	FName WeaponSocket = TEXT("WeaponSocket");
	Weapon = GetWorld()->SpawnActor<AWeapon>(DefaultWapeon, spawnParams);


	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSocket);
	//Weapon->SetOwner(this);
	//Weapon->AttachRootComponentTo(GetMesh(), WeaponSocket, EAttachLocation::SnapToTarget, true);
}

// Called every frame
void ATotalInsanityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Controller && Controller->IsLocalController())
	{
		ATIUsableActor* Usable = GetUsableInView();

		// End Focus
		if (FocusedUsableActor != Usable)
		{
			if (FocusedUsableActor)
			{
				FocusedUsableActor->OnEndFocus();
			}

			bHasNewFocus = true;
		}

		// Assign new Focus
		FocusedUsableActor = Usable;

		// Start Focus.
		if (Usable)
		{
			if (bHasNewFocus)
			{
				Usable->OnBeginFocus();
				bHasNewFocus = false;
			}
		}
	}
}

// Called to bind functionality to input
void ATotalInsanityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	InputComponent->BindAxis("MoveForward", this, &ATotalInsanityCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATotalInsanityCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATotalInsanityCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATotalInsanityCharacter::OnStopJump);

	InputComponent->BindAction("Use", IE_Pressed, this, &ATotalInsanityCharacter::Use);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ATotalInsanityCharacter::OnStartFire);
}

// Interaction with usabele actor
ATIUsableActor* ATotalInsanityCharacter::GetUsableInView()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == NULL)
		return NULL;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<ATIUsableActor>(Hit.GetActor());
}

void ATotalInsanityCharacter::Use()
{
	// Only allow on server. If called on client push this request to the server
	if (Role == ROLE_Authority)
	{
		ATIUsableActor* Usable = GetUsableInView();
		if (Usable)
		{
			Usable->OnUsed(this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ATotalInsanityCharacter::ServerUse_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerUse_Implementation"));
	Use();
}

bool ATotalInsanityCharacter::ServerUse_Validate()
{
	return true;
}

//Jumping
void ATotalInsanityCharacter::OnStartJump()
{
	bPressedJump = true;
	UE_LOG(LogTemp, Warning, TEXT("OnStartJump %s"), *this->GetActorLabel());
	SetIsJumping(true);
}

void ATotalInsanityCharacter::OnStopJump()
{
	UE_LOG(LogTemp, Warning, TEXT("OnStopJump %s"), *this->GetActorLabel());
	bPressedJump = false;
}

bool ATotalInsanityCharacter::IsInitiatedJump() const
{
	return bIsJumping;
}

void ATotalInsanityCharacter::SetIsJumping(bool NewJumping)
{
	// Go to standing pose if trying to jump while crouched
	if (bIsCrouched && NewJumping)
	{
		UnCrouch();
	}
	else
	{
		bIsJumping = NewJumping;
	}

	if (Role != ROLE_Authority)
	{
		ServerSetIsJumping(NewJumping);
	}
}

void ATotalInsanityCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	SetIsJumping(false);
}

void ATotalInsanityCharacter::ServerSetIsJumping_Implementation(bool NewJumping)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *this->GetActorLabel());
	SetIsJumping(NewJumping);
}

bool ATotalInsanityCharacter::ServerSetIsJumping_Validate(bool NewJumping)
{
	return true;
}

void ATotalInsanityCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATotalInsanityCharacter, bIsJumping, COND_SkipOwner);

}

void ATotalInsanityCharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		// Limit pitch when walking or falling
		const bool bLimitRotation = (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling());
		const FRotator Rotation = bLimitRotation ? GetActorRotation() : Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

		AddMovementInput(Direction, Val);
	}
}

void ATotalInsanityCharacter::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

FRotator ATotalInsanityCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void ATotalInsanityCharacter::OnStartFire()
{
	if (Role != ROLE_Authority)
	{
		ServerOnStartFire();
	}
	else
	{
		Weapon->StartFire();
	}
}

void ATotalInsanityCharacter::ServerOnStartFire_Implementation()
{
	Weapon->StartFire();
}

bool ATotalInsanityCharacter::ServerOnStartFire_Validate()
{
	return true;
}