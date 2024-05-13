// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWeapon::SyncFireRate()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AWeapon::Fire, FireRate, true);
}

void AWeapon::Fire()
{
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

