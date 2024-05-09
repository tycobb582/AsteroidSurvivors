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

	LeftBoundCollision->OnComponentBeginOverlap.AddDynamic(this, &AGameBoundary::OnBoundsBeginOverlap);
	TopBoundCollision->OnComponentBeginOverlap.AddDynamic(this, &AGameBoundary::OnBoundsBeginOverlap);
	RightBoundCollision->OnComponentBeginOverlap.AddDynamic(this, &AGameBoundary::OnBoundsBeginOverlap);
	BottomBoundCollision->OnComponentBeginOverlap.AddDynamic(this, &AGameBoundary::OnBoundsBeginOverlap);
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
	

	FVector BoxExtent = FVector(HalfBondaryWidth, 32, 32);
	LeftBoundCollision->SetBoxExtent(BoxExtent);
	TopBoundCollision->SetBoxExtent(BoxExtent);
	RightBoundCollision->SetBoxExtent(BoxExtent);
	BottomBoundCollision->SetBoxExtent(BoxExtent);

	FVector BoundScale = FVector(2 * (HalfBondaryWidth / 100), 1, 1);
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

	TopBoundCollision->SetRelativeLocation(FVector(HalfBoundaryHeight + 100, 0, 0));
	TopBoundCollision->SetRelativeRotation(FRotator(0, 90, 0));
	LeftBoundCollision->SetRelativeLocation(FVector(0, -HalfBondaryWidth - 100, 0));
	RightBoundCollision->SetRelativeLocation(FVector(0, HalfBondaryWidth + 100, 0));
	BottomBoundCollision->SetRelativeLocation(FVector(-HalfBoundaryHeight - 100, 0, 0));
	BottomBoundCollision->SetRelativeRotation(FRotator(0, 90, 0));
}

void AGameBoundary::OnBoundsBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Debug)
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Blue, FString::Printf(TEXT("%s collision with %s"), *OtherActor->GetName(), *OverlappedComp->GetName()));
	const FVector ActorLocation = OtherActor->GetActorLocation();
	FVector NewLocation = ActorLocation;
	const FString CompName = OverlappedComp->GetName();
	if (CompName == "LeftCollision")
		NewLocation.Y = RightBoundCollision->GetComponentLocation().Y - 150;
	else if (CompName == "RightCollision")
		NewLocation.Y = LeftBoundCollision->GetComponentLocation().Y + 150;
	else if (CompName == "TopCollision")
		NewLocation.X = BottomBoundCollision->GetComponentLocation().X + 150;
	else if (CompName == "BottomCollision")
		NewLocation.X = TopBoundCollision->GetComponentLocation().X - 150;
	OtherActor->SetActorLocation(NewLocation);
}