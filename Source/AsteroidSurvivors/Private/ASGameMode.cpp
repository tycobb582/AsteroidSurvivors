// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameMode.h"

#include "PlayerShip.h"

AASGameMode::AASGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Player/BP_PlayerShip"));
	if (IsValid(PlayerPawnBPClass.Class))
		DefaultPawnClass = PlayerPawnBPClass.Class;
}
