// Fill out your copyright notice in the Description page of Project Settings.


#include "Rail.h"
#include "ProjectS21/Grid/CellPoint.h"

ARail::ARail()
{
	mpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	const ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/ProjectS21Assets/Rails/SM_Rail"));
	mpMesh->SetStaticMesh(mesh.Object);
}

void ARail::BeginPlay()
{
	Super::BeginPlay();
}

void ARail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACellPoint* currentCell = Cast<ACellPoint>(GetAttachParentActor());
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::FromInt(currentCell->maNeighbours.Num()));
}
