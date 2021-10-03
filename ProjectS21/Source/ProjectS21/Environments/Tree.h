// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectS21/Season.h"
#include "Components\WidgetComponent.h"
#include "Tree.generated.h"

class APlayerCharacter;
UCLASS()
class PROJECTS21_API ATree : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mpTreeMeshComponent;

	UPROPERTY(EditAnywhere)
		UWidgetComponent* mpInteractWidget;
	
public:	
	// Sets default values for this actor's properties
	ATree();
	virtual ~ATree();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ChangeSeason(const Season& newSeason);
	void CollectResource(APlayerCharacter* pPlayer);

	void ShowInteractionWidget(bool visibility);

private:
	UPROPERTY(EditAnywhere, Category="Tree Mesh")
		UStaticMesh* mpTree;
	UPROPERTY(EditAnywhere, Category="Tree Mesh")
		UStaticMesh* mpFallTree;

	int mAmountOfWood = 2;
	int mAmountOfAcorn = 5;
	int mAmountOfMaple = 3;
	int mAmountOfCherries = 4;

	bool mHasCollectedResource = false;
	Season mCurrentSeason = Season::SPRING;

};
