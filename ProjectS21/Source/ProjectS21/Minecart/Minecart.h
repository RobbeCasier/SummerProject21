// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectS21/Rail/BaseRail.h"
#include "Minecart.generated.h"

UCLASS()
class PROJECTS21_API AMinecart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinecart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetRail(ABaseRail* rail) { mpCurrentRail = rail; }
	ABaseRail* GetRail() { return mpCurrentRail; }
private:
	ABaseRail* mpCurrentRail;
};
