// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Collider = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	Collider->SetupAttachment(RootComponent);
	
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Ship");
	Mesh->SetupAttachment(RootComponent);

	ExhaustParticles = CreateDefaultSubobject<UNiagaraComponent>("ExhaustParticles");
	ExhaustParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APlayerShip::ShipMovement(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector.Normalize();
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	
	float RotationInput = MovementVector.X;
	FRotator ShipRotator = FRotator(0, RotationInput * RotationSpeed * DeltaSeconds, 0);
	AddActorLocalRotation(ShipRotator);
	
	const float MovementInput = MovementVector.Y;
	float NewMoveSpeed = MovementSpeed;
	if (MovementInput > 0)
		NewMoveSpeed += Acceleration * DeltaSeconds;
	else if (MovementInput < 0)
		NewMoveSpeed -= Acceleration * DeltaSeconds;
	MovementSpeed = UKismetMathLibrary::FClamp(NewMoveSpeed, 0, TopSpeed);
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float ParticleSpawnRate = 50 * (MovementSpeed / TopSpeed);
	ExhaustParticles->SetFloatParameter(FName("SpawnRate"), ParticleSpawnRate);
	
	if (MovementSpeed)
	{
		FVector NewLocation = GetActorLocation() + GetActorForwardVector() * (MovementSpeed * DeltaTime);
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerShip::ShipMovement);	
}

