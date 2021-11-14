// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CApplianceMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOASTERWARS_API UCApplianceMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UCApplianceMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Accumulated impulse to be added next tick. */
	UPROPERTY()
		FVector PendingImpulseToApply;
};
