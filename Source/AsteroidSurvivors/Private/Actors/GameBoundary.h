// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameBoundary.generated.h"

UCLASS()
class AGameBoundary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoundary();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnBoundsEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* LeftBoundCollision;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftBound;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TopBoundCollision;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TopBound;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* RightBoundCollision;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightBound;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BottomBoundCollision;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BottomBound;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Debug;
};
