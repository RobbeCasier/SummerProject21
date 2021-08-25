// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mpSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mpSpringArm->SetupAttachment(RootComponent);
	mpSpringArm->SetRelativeRotation({ -45, 0, 0 });
	mpSpringArm->SetRelativeLocation({ 0, 0, 20 });
	mpSpringArm->bInheritPitch = false;
	mpSpringArm->bInheritYaw = false;
	mpSpringArm->bInheritRoll = false;
	mpSpringArm->TargetArmLength = 500.f;

	mpCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPCamera"));
	mpCamera->SetupAttachment(mpSpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = { 0, 1000, 0 };
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	USkeletalMeshComponent* skeletalMesh = GetMesh();
	skeletalMesh->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin")));
	skeletalMesh->SetRelativeRotation({ 0, -90, 0 });
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::MoveForward(float inputVal)
{
	FVector forward = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	AddMovementInput(forward, inputVal);
}

void APlayerCharacter::MoveRight(float inputVal)
{
	FVector right = UKismetMathLibrary::GetRightVector(GetControlRotation());
	AddMovementInput(right, inputVal);
}

