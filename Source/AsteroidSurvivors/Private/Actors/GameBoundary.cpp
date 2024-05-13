// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameBoundary.h"
#include "../ASGameMode.h"

// Sets default values
AGameBoundary::AGameBoundary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	LeftBoundCollision = CreateDefaultSubobject<UBoxComponent>("LeftCollision");
	LeftBoundCollision->SetupAttachment(Root);
	TopBoundCollision = CreateDefaultSubobject<UBoxComponent>("TopCollision");
	TopBoundCollision->SetupAttachment(Root);
	RightBoundCollision = CreateDefaultSubobject<UBoxComponent>("RightCollision");
	RightBoundCollision->SetupAttachment(Root);
	BottomBoundCollision = CreateDefaultSubobject<UBoxComponent>("BottomCollision");
	BottomBoundCollision->SetupAttachment(Root);

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFile(TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* CubeMesh = CubeFile.Object;
	
	LeftBound = CreateDefaultSubobject<UStaticMeshComponent>("LeftBoundVis");
	LeftBound->SetupAttachment(LeftBoundCollision);
	LeftBound->SetStaticMesh(CubeMesh);
	TopBound = CreateDefaultSubobject<UStaticMeshComponent>("TopBoundVis");
	TopBound->SetupAttachment(TopBoundCollision);
	TopBound->SetStaticMesh(CubeMesh);
	RightBound = CreateDefaultSubobject<UStaticMeshComponent>("RightBoundVis");
	RightBound->SetupAttachment(RightBoundCollision);
	RightBound->SetStaticMesh(CubeMesh);
	BottomBound = CreateDefaultSubobject<UStaticMeshComponent>("BottomBoundVis");
	BottomBound->SetStaticMesh(CubeMesh);
	BottomBound->SetupAttachment(BottomBoundCollision);

	LeftBoundCollision->OnComponentEndOverlap.AddDynamic(this, &AGameBoundary::OnBoundsEndOverlap);
	TopBoundCollision->OnComponentEndOverlap.AddDynamic(this, &AGameBoundary::OnBoundsEndOverlap);
	RightBoundCollision->OnComponentEndOverlap.AddDynamic(this, &AGameBoundary::OnBoundsEndOverlap);
	BottomBoundCollision->OnComponentEndOverlap.AddDynamic(this, &AGameBoundary::OnBoundsEndOverlap);
}

void AGameBoundary::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UWorld* World = GetWorld();
	int BoundaryWidth = 1;
	if (IsValid(World))
	{
		AASGameMode* GM = Cast<AASGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GM))
			BoundaryWidth = GM->GetWorldSize();
	}
	float HalfBondaryWidth = static_cast<float>(BoundaryWidth) / 2;
	float HalfBoundaryHeight = static_cast<float>(BoundaryWidth) * 0.28125;
	

	const FVector BoxExtent = FVector(HalfBondaryWidth, 32, 300);
	LeftBoundCollision->SetBoxExtent(BoxExtent);
	TopBoundCollision->SetBoxExtent(BoxExtent);
	RightBoundCollision->SetBoxExtent(BoxExtent);
	BottomBoundCollision->SetBoxExtent(BoxExtent);

	const FVector BoundScale = FVector(2 * (HalfBondaryWidth / 100), 1, 1);
	LeftBound->SetHiddenInGame(true);
	LeftBound->SetCollisionProfileName("NoCollision");
	LeftBound->SetRelativeScale3D(BoundScale);
	TopBound->SetHiddenInGame(true);
	TopBound->SetCollisionProfileName("NoCollision");
	TopBound->SetRelativeScale3D(BoundScale);
	RightBound->SetHiddenInGame(true);
	RightBound->SetCollisionProfileName("NoCollision");
	RightBound->SetRelativeScale3D(BoundScale);
	BottomBound->SetHiddenInGame(true);
	BottomBound->SetCollisionProfileName("NoCollision");
	BottomBound->SetRelativeScale3D(BoundScale);

	TopBoundCollision->SetRelativeLocation(FVector(HalfBoundaryHeight, 0, 0));
	TopBoundCollision->SetRelativeRotation(FRotator(0, 90, 0));
	LeftBoundCollision->SetRelativeLocation(FVector(0, -HalfBondaryWidth, 0));
	RightBoundCollision->SetRelativeLocation(FVector(0, HalfBondaryWidth, 0));
	BottomBoundCollision->SetRelativeLocation(FVector(-HalfBoundaryHeight, 0, 0));
	BottomBoundCollision->SetRelativeRotation(FRotator(0, 90, 0));
}

void AGameBoundary::OnBoundsEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FVector ActorLocation = OtherActor->GetActorLocation();
	if (OverlappedComp == LeftBoundCollision && ActorLocation.Y >= LeftBoundCollision->GetComponentLocation().Y)
		return;
	if (OverlappedComp == RightBoundCollision && ActorLocation.Y <= RightBoundCollision->GetComponentLocation().Y)
		return;
	if (OverlappedComp == TopBoundCollision && ActorLocation.X <= TopBoundCollision->GetComponentLocation().X)
		return;
	if (OverlappedComp == BottomBoundCollision && ActorLocation.X >= BottomBoundCollision->GetComponentLocation().X)
		return;
	
	FVector NewLocation = ActorLocation;
	if (OverlappedComp == LeftBoundCollision)
		NewLocation.Y = RightBoundCollision->GetComponentLocation().Y;
	else if (OverlappedComp == RightBoundCollision)
		NewLocation.Y = LeftBoundCollision->GetComponentLocation().Y;
	else if (OverlappedComp == TopBoundCollision)
		NewLocation.X = BottomBoundCollision->GetComponentLocation().X;
	else if (OverlappedComp == BottomBoundCollision)
		NewLocation.X = TopBoundCollision->GetComponentLocation().X;
	OtherActor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	if (Debug)
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Blue, FString::Printf(TEXT("%s teleport from %s"), *OtherActor->GetName(), *OverlappedComp->GetName()));
}