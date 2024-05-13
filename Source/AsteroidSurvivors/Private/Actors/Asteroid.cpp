// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Asteroid.h"

#include "ASGameMode.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Collider = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	Collider->SetupAttachment(RootComponent);
	Collider->SetCollisionProfileName("AsteroidCollision");
	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Laser Mesh");
	AsteroidMesh->SetupAttachment(Collider);
	AsteroidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Direction = FMath::VRand();
	Direction.Z = 0;
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	FVector StartLocation = GetActorLocation();
	FVector Throwaway;
	FVector AsteroidBounds;
	GetActorBounds(true, Throwaway, AsteroidBounds);
	StartLocation.Z = AsteroidBounds.Z;
	SetActorLocation(StartLocation);
}

void AAsteroid::InitializeAsteroid(EAsteroidRank InitialRank)
{
	Rank = InitialRank;
	switch (Rank)
	{
	case EAsteroidRank::Split:
		Health = InitialHealth * 0.66;
		Speed = InitialSpeed * 1.25;
		break;
	case EAsteroidRank::Bit:
		Health = InitialHealth * 0.33;
		Speed = InitialSpeed * 1.5;
		AsteroidDestroyed.AddDynamic(Cast<AASGameMode>(GetWorld()->GetAuthGameMode()), &AASGameMode::OnAsteroidDestroyed);
		break;
	default:
		break;
	}
}

void AAsteroid::OnHit(const int LaserDamage)
{
	Health -= LaserDamage;
	if (Health <= 0)
		Explode();
}

void AAsteroid::Explode()
{
	EAsteroidRank NewAsteroidRank;
	FVector NewAsteroidScale;
	switch (Rank)
	{
		case EAsteroidRank::Full:
			AsteroidDestroyed.Broadcast(Rank);
			NewAsteroidRank = EAsteroidRank::Split;
			NewAsteroidScale = FVector(0.75, 0.75, 0.75);
			break;
		case EAsteroidRank::Split:
			AsteroidDestroyed.Broadcast(Rank);
			NewAsteroidRank = EAsteroidRank::Bit;
			NewAsteroidScale = FVector(0.5, 0.5, 0.5);
			break;
		default:
			AsteroidDestroyed.Broadcast(Rank);
			Destroy();
			return;
	}
	for (int i = 0; i < 2; i++)
	{
		FTransform NewAsteroidTransform = FTransform(GetActorRotation(), GetActorLocation(), NewAsteroidScale);
		AAsteroid* SmallerAsteroid = GetWorld()->SpawnActorDeferred<AAsteroid>(this->GetClass(), NewAsteroidTransform);
		SmallerAsteroid->InitializeAsteroid(NewAsteroidRank);
		SmallerAsteroid->FinishSpawning(NewAsteroidTransform);
	}
	Destroy();
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewPos = GetActorLocation() + (Speed * Direction * DeltaTime);
	SetActorLocation(NewPos);
}

