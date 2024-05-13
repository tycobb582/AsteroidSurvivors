// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Laser.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UFUNCTION(BlueprintCallable)
	void SyncFireRate();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALaser> WeaponLaser;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle FireTimer;
};
