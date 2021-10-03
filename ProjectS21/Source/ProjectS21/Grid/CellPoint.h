// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Containers/Map.h"
#include "Materials/Material.h"
#include "CellPoint.generated.h"

UCLASS()
class PROJECTS21_API ACellPoint : public AActor
{
	GENERATED_BODY()

	USceneComponent* mpLocationComponent;
public:	
	// Sets default values for this actor's properties
	ACellPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update();
	void Select();
	void Deselect();

	ACellPoint* GetNeighbour(const FVector& direction);

	UPROPERTY(Editanywhere, Category = "Points")
	TMap<FVector, ACellPoint*> maNeighbours;

	UPROPERTY(Editanywhere, Category = "Grid Actor")
		UClass* mpSpawnClass;

private:
	UClass* mpPreviousSpawnClass;

	UStaticMeshComponent* mpSelectionPlane;
	UMaterialInstance* mpSelectionMat;
};
