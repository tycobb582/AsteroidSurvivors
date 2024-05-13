// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponsSystem.h"

// Sets default values for this component's properties
UWeaponsSystem::UWeaponsSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWeaponsSystem::SyncWeaponFireRates()
{
	for (const auto Weapon : ActiveWeapons)
		Weapon->SyncFireRate();
}

void UWeaponsSystem::AddWeapon(const TSubclassOf<AWeapon> NewWeapon)
{
	if (!OwnedWeaponTypes.Contains(NewWeapon))
		OwnedWeaponTypes.Add(NewWeapon);
	
	AActor* OwningShip = GetOwner();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningShip;
	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(NewWeapon, OwningShip->GetActorLocation(), FRotator(0, 0, 0), SpawnParams);
	SpawnedWeapon->AttachToActor(OwningShip, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ActiveWeapons.Add(SpawnedWeapon);

	SyncWeaponFireRates();
}


// Called when the game starts
void UWeaponsSystem::BeginPlay()
{
	Super::BeginPlay();

	for (const auto WeaponClass : OwnedWeaponTypes)
		AddWeapon(WeaponClass);
}


// Called every frame
void UWeaponsSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

