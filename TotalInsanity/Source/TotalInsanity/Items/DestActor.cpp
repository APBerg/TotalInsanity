// Copyright Adam Berg 2017

#include "DestActor.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SphereComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADestActor::ADestActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bReplicateMovement = true;
	Strength = 300.0f;
	Radius = 500.0f;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
	SphereComp = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetSphereRadius(Radius);
	SphereComp->SetupAttachment(MeshComp);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	MaxHealth = 100.0f;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void ADestActor::BeginPlay()
{
	Super::BeginPlay();

	DynamicMAT = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
}

// Called every frame
void ADestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Health<0)
	{
		Health = 0.0f;
	}
	DynamicMAT->SetVectorParameterValue(FName("GlowColour"), FLinearColor(((MaxHealth - Health) / MaxHealth), Health / MaxHealth, 0.0f, 1.0f));

}

float ADestActor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Role == ROLE_Authority)
	{
		Health = Health - DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage taken from %i. Health: %f/%f"), EventInstigator->PlayerState->PlayerId, Health, MaxHealth);
	
		if (Health <= 0)
		{
			TArray<AActor*> NearbyActors;
			SphereComp->GetOverlappingActors(NearbyActors);
			for (auto It = NearbyActors.CreateIterator(); It; It++)
			{
				//Get the static mesh component for each actor
				UStaticMeshComponent* SM = Cast<UStaticMeshComponent>((*It)->GetRootComponent());
				if (SM)
				{
					SM->AddRadialImpulse(GetActorLocation(), Radius, Strength, ERadialImpulseFalloff::RIF_Linear, true);
				}
			}
			EventInstigator->PlayerState->Score++;
			SimulateExplosionFX();
			SetLifeSpan(1.0f);
		}
	}
	return DamageAmount;
}

void ADestActor::OnRep_Health()
{
	//UpdateText();
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Health Health: %f/%f"), Health, MaxHealth);

}

void ADestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADestActor, Health);
}

void ADestActor::SimulateExplosionFX_Implementation()
{
	if (ExplosionFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetTransform(), true);
		MeshComp->SetVisibility(false);
	}
}
