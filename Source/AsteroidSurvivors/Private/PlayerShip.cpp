// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Asteroid.h"
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

	WeaponsSystem = CreateDefaultSubobject<UWeaponsSystem>("Weapons System");
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

void APlayerShip::BeginThrust()
{
	Velocity += GetActorForwardVector() * 100;
	ClampSpeed();
}

void APlayerShip::ShipMovement(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	InputVector.Normalize();
	const float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	
	const float RotationInput = InputVector.X;
	const FRotator ShipRotator = FRotator(0, RotationInput * RotationSpeed * DeltaSeconds, 0);
	AddActorLocalRotation(ShipRotator);

	if (InputVector.Y)
	{
		Acceleration = GetActorForwardVector() * AccelerationRate;
		IsAccelerating = true;
		Velocity += Acceleration * DeltaSeconds;
		ClampSpeed();
		const float MovementSpeed = Velocity.Length();
		const float ParticleSpawnRate = 50 * (MovementSpeed / TopSpeed);
		ExhaustParticles->SetFloatParameter(FName("SpawnRate"), ParticleSpawnRate);
	}
	else
		IsAccelerating = false;
}

void APlayerShip::EndAcceleration(const FInputActionValue& Value)
{
	IsAccelerating = false;
	ExhaustParticles->SetFloatParameter(FName("SpawnRate"), 0);
}

void APlayerShip::ClampSpeed()
{
	const FVector ShipDir = Velocity.GetSafeNormal();
	const FVector MaxVelocity = ShipDir * TopSpeed;
	Velocity.X = UKismetMathLibrary::FClamp(Velocity.X, FMath::Min(MaxVelocity.X, 0), FMath::Max(MaxVelocity.X, 0));
	Velocity.Y = UKismetMathLibrary::FClamp(Velocity.Y, FMath::Min(MaxVelocity.Y, 0), FMath::Max(MaxVelocity.Y, 0));
}

void APlayerShip::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (const AAsteroid* AsteroidHit = Cast<AAsteroid>(OtherActor))
	{
		Health -= AsteroidHit->ImpactDamage;
		Velocity *= - 1;
		ClampSpeed();
		if (Health <= 0)
			DisplayGameOverScreen();
	}
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAccelerating)
	{
		Velocity -= Velocity.GetSafeNormal() * DragAccelerationRate * DeltaTime;
		ClampSpeed();
	}

	if (Velocity.X or Velocity.Y)
	{
		const FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
		SetActorLocation(NewLocation);	
	}
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &APlayerShip::BeginThrust);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerShip::ShipMovement);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerShip::EndAcceleration);
}

