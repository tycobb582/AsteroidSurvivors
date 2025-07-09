// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

UENUM(BlueprintType)
enum class EAsteroidRank : uint8
{
	Full,
	Split,
	Bit
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsteroidDestroyedDelegate, EAsteroidRank, Rank, int, Xp);

UCLASS()
class AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeAsteroid(EAsteroidRank InitialRank);
	
	void Explode();
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AsteroidMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnHit(const int LaserDamage);

	UPROPERTY(EditAnywhere, Category= "Asteroid|Stats")
	int InitialHealth = 100;

	UPROPERTY(BlueprintReadWrite)
	int Health = InitialHealth;

	UPROPERTY(EditAnywhere, Category= "Asteroid|Stats")
	float InitialSpeed = 500;

	UPROPERTY(BlueprintReadWrite)
	float Speed = InitialSpeed;

	UPROPERTY(EditAnywhere, Category= "Asteroid|Stats")
	int ImpactDamage = 34;

	UPROPERTY(EditAnywhere, Category= "Asteroid|Stats")
	int XpReward = 25;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Direction = FVector(1, 0, 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EAsteroidRank Rank = EAsteroidRank::Full;

	FAsteroidDestroyedDelegate AsteroidDestroyed;
};
