// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon.h"
#include "ForwardCannon.generated.h"

/**
 * 
 */
UCLASS()
class AForwardCannon : public AWeapon
{
	GENERATED_BODY()

public:
	AForwardCannon();
	
	virtual void Fire() override;
	
};
