// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameMode.h"
#include "Actors/Asteroid.h"
#include "Kismet/GameplayStatics.h"

AASGameMode::AASGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/BP_PlayerShip"));
	if (IsValid(PlayerPawnBPClass.Class))
		DefaultPawnClass = PlayerPawnBPClass.Class;
}

void AASGameMode::BeginPlay()
{
	Super::BeginPlay();

	SpawnWave();
}

void AASGameMode::SpawnWave()
{
	WaveNum++;
	const int NumToSpawn = FMath::RoundToInt(1.5 * FMath::Log2(static_cast<float>(WaveNum)) + 3);
	const APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector PlayerLocation = Player->GetActorLocation();
	PlayerLocation.Z = 0;
	for (int i = 0; i < NumToSpawn; i++)
	{
		FVector SpawnLocation;
		do
		{
			SpawnLocation = FMath::VRand();
			SpawnLocation.Z = 0;
			SpawnLocation *= FMath::FRandRange(-3900.0, 3900.0);
		}
		while (FVector::Dist(SpawnLocation, PlayerLocation) <= 400);
		const FTransform SpawnTransform = FTransform(SpawnLocation);
		GetWorld()->SpawnActor<AAsteroid>(AsteroidToSpawn, SpawnTransform);
	}
	ActiveAsteroids = NumToSpawn * 4;
}

void AASGameMode::OnAsteroidDestroyed(EAsteroidRank Rank)
{
	if (Rank == EAsteroidRank::Bit)
	{
		ActiveAsteroids--;
		if (ActiveAsteroids == 0)
			SpawnWave();	
	}
}

int AASGameMode::GetWorldSize()
{
	return WorldSize;
}
