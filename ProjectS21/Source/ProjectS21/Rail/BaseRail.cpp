// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRail.h"

// Sets default values
ABaseRail::ABaseRail()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseRail::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseRail::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
