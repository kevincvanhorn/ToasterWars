// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CAppliancePawn.generated.h"

UCLASS()
class TOASTERWARS_API ACAppliancePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACAppliancePawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAcces = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAcces = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAcces = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	class UCApplianceMovementComponent* MovementComponent;

	virtual class UPawnMovementComponent* GetMovementComponent() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAcces = "true"))
		float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAcces = "true"))
		float JumpScale;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected: 
	UFUNCTION(Server, Reliable)
	virtual void RotatePawn(float AxisValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "UsePrimaryAction"))
	void UsePrimaryActionEvent();

	UFUNCTION(Server, Reliable)
	virtual void UsePrimaryAction();
};
