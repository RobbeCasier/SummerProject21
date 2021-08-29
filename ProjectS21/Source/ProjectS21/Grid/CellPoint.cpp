// Fill out your copyright notice in the Description page of Project Settings.


#include "CellPoint.h"

// Sets default values
ACellPoint::ACellPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mpLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location"));
	RootComponent = mpLocationComponent;
}

// Called when the game starts or when spawned
void ACellPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACellPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ACellPoint::Update()
{
	if (mpSpawnClass != mpPreviousSpawnClass)
	{
		const FVector location = GetActorLocation();
		auto actor = GetWorld()->SpawnActor(mpSpawnClass, &location);
		actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		mpPreviousSpawnClass = mpSpawnClass;
	}
}

