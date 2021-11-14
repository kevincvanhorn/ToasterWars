// Fill out your copyright notice in the Description page of Project Settings.

#include "CAppliancePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Player/CApplianceMovementComponent.h"
#include "Public/CProjectileActor.h"

// https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/Components/

// Sets default values
ACAppliancePawn::ACAppliancePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	// Root Component is a static mesh component that reacts to physics:
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	if (ensure(MeshComponent)) {
		RootComponent = MeshComponent;
		MeshComponent->SetSimulatePhysics(true); // For using gravity on this static mesh
		MeshComponent->SetConstraintMode(EDOFMode::YZPlane);
		MeshComponent->SetIsReplicated(true);
	}

	// Spring Arm & Camera:
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (ensure(Camera && SpringArm)) {
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	}

	// Movement Component:
	MovementComponent = CreateDefaultSubobject<UCApplianceMovementComponent>(TEXT("MovementComponent"));
	if (ensure(MovementComponent)) {
		MovementComponent->UpdatedComponent = RootComponent;

		// Defaults:
		RotationSpeed = 1.0f;
		JumpScale = 1.0f;
	}

}

UPawnMovementComponent* ACAppliancePawn::GetMovementComponent() const
{
	return MovementComponent;
}

// Called when the game starts or when spawned
void ACAppliancePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACAppliancePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACAppliancePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ACAppliancePawn::Fire);
	PlayerInputComponent->BindAction("PrimaryAction", EInputEvent::IE_Pressed, this, &ACAppliancePawn::UsePrimaryAction);
	PlayerInputComponent->BindAxis("Rotate", this, &ACAppliancePawn::RotatePawn);
}

void ACAppliancePawn::RotatePawn_Implementation(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	//NewRotation.Roll += AxisValue* RotationSpeed;
	//SetActorRotation(NewRotation);

	MeshComponent->AddAngularImpulseInDegrees(FVector(AxisValue*RotationSpeed*-1,0,0), NAME_None, true);
}

void ACAppliancePawn::UsePrimaryAction_Implementation()
{
	if (ensure(MovementComponent && MeshComponent)) {
		this->UsePrimaryActionEvent();
		MeshComponent->AddImpulse(MeshComponent->GetUpVector()*JumpScale*-1.0f, NAME_None, true); // bVelChange => use velocity w/o mass
	}
}

void ACAppliancePawn::Fire() {
	ServerFire();
}

void ACAppliancePawn::ServerFire_Implementation() {
	if (ensureAlways(ProjectileClass)) {
		FVector ProjectileSpawnLoc = MeshComponent->GetSocketLocation("ProjectileSpawn_01");
		FRotator ProjectileSpawnRot = MeshComponent->GetSocketRotation("ProjectileSpawn_01");

		// Spawn collision handling override:
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParams.Instigator = this;

		GetWorld()->SpawnActor<ACProjectileActor>(ProjectileClass, ProjectileSpawnLoc, ProjectileSpawnRot, ActorSpawnParams);
	}
}
