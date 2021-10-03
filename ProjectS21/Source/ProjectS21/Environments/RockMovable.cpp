// Fill out your copyright notice in the Description page of Project Settings.


#include "RockMovable.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ARockMovable::ARockMovable()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* pSceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = pSceneComp;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/DreamscapeSeries/DreamscapeMeadows/Meshes/Rocks/SM_Stone_Small_06"));
	mpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mpMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mpMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	mpMesh->SetStaticMesh(mesh.Object);

	UFloatingPawnMovement* movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	UGridMovement* gridMovement = CreateDefaultSubobject<UGridMovement>(TEXT("GridMovementComponent"));
	gridMovement->SetMovementComponent(movement);
	AddOwnedComponent(movement);
	AddOwnedComponent(gridMovement);
	mpMesh->SetupAttachment(pSceneComp);
}

// Called when the game starts or when spawned
void ARockMovable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARockMovable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARockMovable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

