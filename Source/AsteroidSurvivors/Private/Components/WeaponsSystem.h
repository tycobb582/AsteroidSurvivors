// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon.h"
#include "Components/ActorComponent.h"
#include "WeaponsSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponsSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponsSystem();

	UFUNCTION(BlueprintCallable)
	void SyncWeaponFireRates();

	UFUNCTION(BlueprintCallable)
	void AddWeapon(const TSubclassOf<AWeapon> NewWeapon);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AWeapon>> OwnedWeaponTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AWeapon*> ActiveWeapons;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
