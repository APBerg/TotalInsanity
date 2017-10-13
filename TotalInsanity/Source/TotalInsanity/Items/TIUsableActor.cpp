// Copyright Adam Berg 2017

#include "TIUsableActor.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
ATIUsableActor::ATIUsableActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATIUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATIUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATIUsableActor::OnUsed(APawn* InstigatorPawn)
{

	UE_LOG(LogTemp, Warning, TEXT("OnUsed %s"), *this->GetActorLabel());
	// Nothing to do here...
}


void ATIUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	UE_LOG(LogTemp, Warning, TEXT("%s"), *this->GetActorLabel());
	MeshComp->SetRenderCustomDepth(true);
}


void ATIUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}