// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRail.h"
#include "Rail.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTS21_API ARail : public ABaseRail
{
	GENERATED_BODY()
	
public:
	ARail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
