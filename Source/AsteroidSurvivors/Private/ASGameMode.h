// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Asteroid.h"
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

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnWave();
	
	int WorldSize = 8000;

	UPROPERTY(BlueprintReadOnly)
	int ActiveAsteroids = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAsteroid> AsteroidToSpawn;

public:
	UFUNCTION(BlueprintCallable)
	int GetWorldSize();

	UFUNCTION()
	void OnAsteroidDestroyed(EAsteroidRank AsteroidRank);

	UPROPERTY(BlueprintReadWrite)
	int WaveNum = 0;
};
