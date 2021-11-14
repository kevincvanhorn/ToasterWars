// Fill out your copyright notice in the Description page of Project Settings.


#include "CProjectileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ACProjectileActor::ACProjectileActor()
{
	// Root Component is a static mesh component that reacts to physics:
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	if (ensure(MeshComponent)) {
		RootComponent = MeshComponent;
		MeshComponent->SetCollisionProfileName("Projectile");
		MeshComponent->OnComponentHit.AddDynamic(this, &ACProjectileActor::OnHit);	// When this projectile hits another component.

		// Players can't walk on it
		MeshComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
		MeshComponent->CanCharacterStepUpOn = ECB_No;
		MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// Use a ProjectileMovementComponent to govern this projectile's movement
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
		ProjectileMovement->UpdatedComponent = MeshComponent;
		ProjectileMovement->InitialSpeed = 2000.f;
		ProjectileMovement->MaxSpeed = 2000.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = true;

		MeshComponent->SetSimulatePhysics(true); // For using gravity on this static mesh
		MeshComponent->SetConstraintMode(EDOFMode::YZPlane);
		MeshComponent->SetIsReplicated(true);
	}

	InitialLifeSpan = 3.0f;
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics capable object.
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	// Server/Hosting player is the only entity that should DO actions and events. Clients simply replicate them.
	if (HasAuthority())
	{
		MakeNoise(1.0f, GetInstigator());
		Destroy();
	}
}
