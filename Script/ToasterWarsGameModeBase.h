// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToasterWarsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOASTERWARS_API AToasterWarsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;

};
