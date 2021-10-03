// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRail.h"
#include "CrossRail.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTS21_API ACrossRail : public ABaseRail
{
	GENERATED_BODY()

public:
	ACrossRail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FVector GetNextDirection(const FVector& direction);
	virtual USplineComponent* GetTrack(const FVector& direction) override;

};
