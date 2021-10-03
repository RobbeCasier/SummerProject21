// Fill out your copyright notice in the Description page of Project Settings.


#include "Minecart.h"
#include "MinecartMovement.h"

// Sets default values
AMinecart::AMinecart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* pSceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = pSceneComp;

	USkeletalMeshComponent* mpSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/ProjectS21Assets/Minecart/SK_Minecart"));
	mpSKMesh->SetSkeletalMesh(mesh.Object);
	mpSKMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mpSKMesh->SetRelativeLocationAndRotation({ 0,0,48 }, { 0,90,0 });
	mpSKMesh->SetRelativeScale3D({ 0.19,0.19,0.19 });
	mpSKMesh->SetupAttachment(pSceneComp);

	UMinecartMovement* pMovement = CreateDefaultSubobject<UMinecartMovement>(TEXT("MinecartMovementComponent"));
	AddOwnedComponent(pMovement);
}

// Called when the game starts or when spawned
void AMinecart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinecart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinecart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

