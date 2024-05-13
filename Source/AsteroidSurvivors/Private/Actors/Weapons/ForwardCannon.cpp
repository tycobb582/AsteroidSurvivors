// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/ForwardCannon.h"

AForwardCannon::AForwardCannon()
{
	const ConstructorHelpers::FClassFinder<ALaser> LaserClass(TEXT("/Game/Actors/Lasers/BP_ForwardCannonLaser"));
	WeaponLaser = LaserClass.Class;
	FireRate = 1.0;
	WeaponDamage = 35;
}

void AForwardCannon::Fire()
{
	Super::Fire();
	
	FVector OwnerLocation = Owner->GetActorLocation();
	FRotator OwnerRotation = Owner->GetActorRotation();
	FVector FireDirection = Owner->GetActorForwardVector();
	const FTransform SpawnTransform = FTransform(OwnerRotation, OwnerLocation);
	ALaser* SpawnedLaser = GetWorld()->SpawnActorDeferred<ALaser>(WeaponLaser, SpawnTransform);
	SpawnedLaser->InitializeLaser(FireDirection, WeaponDamage);
	SpawnedLaser->FinishSpawning(SpawnTransform);
}
