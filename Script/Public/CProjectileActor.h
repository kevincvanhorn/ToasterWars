// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProjectileActor.generated.h"

UCLASS()
class TOASTERWARS_API ACProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACProjectileActor();

	/** Called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAcces = "true"))
		UStaticMeshComponent* MeshComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		class UProjectileMovementComponent* ProjectileMovement;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
