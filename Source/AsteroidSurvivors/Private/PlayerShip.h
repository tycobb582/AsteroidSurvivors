// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WeaponsSystem.h"
#include "PlayerShip.generated.h"

UCLASS()
class APlayerShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere)
	UWeaponsSystem* WeaponsSystem;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ExhaustParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerShip | Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerShip | Input")
	UInputAction* MoveAction;

	void ShipMovement(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerShip | Movement")
	float RotationSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerShip | Movement")
	float MovementSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerShip | Movement")
	float TopSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerShip | Movement")
	float Acceleration = 5.0f;
};
