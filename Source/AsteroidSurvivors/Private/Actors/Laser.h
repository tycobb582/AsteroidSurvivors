// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

	void InitializeLaser(const FVector Direction, const int Damage);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere, Category= "Laser | Setup")
	float LaserSpeed;

	UPROPERTY(EditAnywhere, Category= "Laser | Setup")
	float LaserLifetime;

	int LaserDamage;

	FTimerHandle LifetimeTimer;

	UPROPERTY(BlueprintReadWrite)
	FVector LaserDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
