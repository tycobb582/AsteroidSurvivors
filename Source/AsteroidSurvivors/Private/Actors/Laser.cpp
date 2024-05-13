// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Laser.h"

#include "Asteroid.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Collider = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	Collider->SetupAttachment(RootComponent);
	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>("Laser Mesh");
	LaserMesh->SetupAttachment(RootComponent);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ALaser::OnBeginOverlap);
}

void ALaser::InitializeLaser(const FVector Direction, const int Damage)
{
	LaserDirection = Direction;
	LaserDamage = Damage;
}

void ALaser::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LaserLifetime);
}

void ALaser::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAsteroid* AsteroidHit = Cast<AAsteroid>(OtherActor);
	if (IsValid(AsteroidHit))
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AsteroidHit->OnHit(LaserDamage);
		Destroy();
	}
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentPos = GetActorLocation();
	FVector NewPos = CurrentPos + (LaserSpeed * LaserDirection) * DeltaTime;
	SetActorLocation(NewPos);
}
