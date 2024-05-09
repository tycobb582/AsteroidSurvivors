// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AASGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AASGameMode();

private:
	int WorldSize = 8000;

public:
	UFUNCTION(BlueprintCallable)
	int GetWorldSize();
};
