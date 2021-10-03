// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectS21/Environments/Tree.h"
#include "Blueprint/UserWidget.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
//#include "Misc/FileHelper.h"
//#include "Serialization/JsonReader.h"
//#include "Serialization/JsonSerializer.h"
//#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectS21/Environments/Tree.h"

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

	mpSeasonParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FallingLeavesEmmiter"));

	const ConstructorHelpers::FObjectFinder<UParticleSystem> fallingLeavesSystem(TEXT("/Game/ProjectS21Assets/Seasons/P_FallingLeaves"));
	mpSeasonParticleEmitter->Template = fallingLeavesSystem.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInstance> fall(TEXT("/Game/ProjectS21Assets/Seasons/MI_Leaf_Fall"));
	mpLeafFall_Mat = fall.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInstance> spring(TEXT("/Game/ProjectS21Assets/Seasons/MI_Leaf_Spring"));
	mpLeafSpring_Mat = spring.Object;

	const ConstructorHelpers::FObjectFinder<UMaterialInstance> winter(TEXT("/Game/ProjectS21Assets/Seasons/MI_Snowflake"));
	mpSnowflake_Mat = winter.Object;

	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true;
	characterMovement->RotationRate = { 0, 1000, 0 };
	//characterMovement->MaxStepHeight = 25.f;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	mpSeasonParticleEmitter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	mpSeasonParticleEmitter->SetRelativeLocation(mParticleRelativeLocation);

	InitItems();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateEnvironment();
	//only check when the player is not controlling something
	//prevent mpSelectedMovementComponent to be nullptr when controlling
	if (!mIsControlling)
		CheckForMovableObject();

	CheckForMinecart();

	FindClosestTree();
	for (FItemStack stack : mItemList)
	{
		FString message = stack.Item.Name + ": " + FString::FromInt(stack.Amount);
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, message);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Control", EInputEvent::IE_Pressed, this, &APlayerCharacter::Control);
	PlayerInputComponent->BindAction("Control", EInputEvent::IE_Released, this, &APlayerCharacter::UnControl);

	PlayerInputComponent->BindAction("Rotate", EInputEvent::IE_Released, this, &APlayerCharacter::RotateObject);

	PlayerInputComponent->BindAction("Push", EInputEvent::IE_Released, this, &APlayerCharacter::Push);

	PlayerInputComponent->BindAction("SwitchSeason", EInputEvent::IE_Released, this, &APlayerCharacter::SwitchSeason);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &APlayerCharacter::Interact);
}

void APlayerCharacter::CheckForMovableObject()
{
	UCapsuleComponent* capsule = GetCapsuleComponent();
	float horizontalLength = 200.f;
	float capsuleHeight = capsule->GetScaledCapsuleHalfHeight() * 2;
	//hypothenusa
	float directionLength = sqrt(pow(capsuleHeight, 2) + pow(horizontalLength, 2));

	FVector start, end;
	FVector direction;
	FHitResult result;
	FCollisionQueryParams colPar;
	colPar.AddIgnoredActor(this);

	start = GetActorLocation();
	direction = GetActorForwardVector();

	float angle = atan2f(horizontalLength, capsuleHeight) * 180 / PI;
	direction = direction.RotateAngleAxis(angle, GetActorRightVector());

	start.Z += capsule->GetScaledCapsuleHalfHeight();
	end = start + (direction * directionLength);

	auto oldRail = mpBrokenRail;

	//line trace
	if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_Pawn, colPar))
	{
		//does result has a UGridMovement
		UGridMovement* component = result.Actor.Get()->FindComponentByClass<UGridMovement>();
		if (component)
		{
			TArray<AActor*> attachedActors;
			result.Actor.Get()->GetAttachedActors(attachedActors);
			auto item = attachedActors.FindByPredicate([](AActor* pActor)
				{
					if (Cast<AMinecart>(pActor))
						return true;
					return false;
				});
			if (item)
			{
				if (mpSelectedMovementComponent)
					Deselect();
				return;
			}
			//deselect previous 
			if (mpSelectedMovementComponent && mpSelectedMovementComponent != component)
			{
				Deselect();
			}

			mpSelectedMovementComponent = component;
			mpBrokenRail = Cast<ABaseRail>(result.Actor.Get());
			mpBrokenRail->ShowWidget(true);

			AActor* pParent = result.Actor.Get()->GetAttachParentActor();
			ACellPoint* pCell = (pParent) ? Cast<ACellPoint>(pParent) : nullptr;
			if (pCell)
				pCell->Select();

			if (oldRail != mpBrokenRail)
			{
				if (oldRail) oldRail->ShowWidget(false);
			}

			return;
		}
	}

	if (oldRail) oldRail->ShowWidget(false);

	if (mpSelectedMovementComponent)
	{
		Deselect();
		mpSelectedMovementComponent = nullptr;
	}

}

void APlayerCharacter::CheckForMinecart()
{
	float directionLength = 200.f;
	FVector start, end;
	FVector direction;
	FHitResult result;
	FCollisionQueryParams colPar;
	colPar.AddIgnoredActor(this);

	start = GetActorLocation();
	direction = GetActorForwardVector();
	end = start + (direction * directionLength);

	if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_Pawn, colPar))
	{
		UMinecartMovement* component = result.Actor.Get()->FindComponentByClass<UMinecartMovement>();
		if (component)
		{
			mpSelectedMinecartMovement = component;
		}
	}
	else
		mpSelectedMinecartMovement = nullptr;
}

void APlayerCharacter::FindClosestTree()
{
	TArray<AActor*> apListOfTrees;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATree::StaticClass(), apListOfTrees);
	float treeSearchRadius = 200.f;

	FVector playerLocation = GetActorLocation();
	float closestDistance = treeSearchRadius;
	auto oldTree = mpClosestTree;
	mpClosestTree = nullptr;
	for (auto actor : apListOfTrees)
	{
		FVector treeLocation = actor->GetActorLocation();
		float distance = FVector::Distance(playerLocation, treeLocation);
		if (distance <= closestDistance)
		{
			closestDistance = distance;
			mpClosestTree = Cast<ATree>(actor);
		}
	}

	if (oldTree != mpClosestTree)
	{
		if (oldTree) oldTree->ShowInteractionWidget(false);
		if (mpClosestTree) mpClosestTree->ShowInteractionWidget(true);
	}
}

void APlayerCharacter::MoveObject(const FVector& direction)
{
	ACellPoint* currentCell = Cast<ACellPoint>(mpSelectedMovementComponent->GetOwner()->GetAttachParentActor());
	if (currentCell)
	{
		ACellPoint* nextCell = currentCell->GetNeighbour(direction);
		if (nextCell)
		{
			TArray<AActor*> childActors;
			nextCell->GetAttachedActors(childActors);
			if (childActors.Num() == 0)
				mpSelectedMovementComponent->Move(nextCell, direction);
		}
	}
}

void APlayerCharacter::Deselect()
{
	AActor* pParent = mpSelectedMovementComponent->GetOwner()->GetAttachParentActor();
	ACellPoint* pCell = (pParent) ? Cast<ACellPoint>(pParent) : nullptr;
	if (pCell)
		pCell->Deselect();
}

void APlayerCharacter::UpdateEnvironment()
{
	for (TObjectIterator<ATree> it; it; ++it)
	{
		ATree* pTree = (*it);
		if (!pTree)
			return;

		FVector treeLocation = pTree->GetActorLocation();
		FVector playerLocation = GetActorLocation();

		float distance = FVector::Distance(playerLocation, treeLocation);

		if (distance <= mSeasonRadius)
			pTree->ChangeSeason(mCurrentSeason);
	}
}

void APlayerCharacter::InitItems()
{
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/ProjectS21/Items.json";
	FString jsonString;

	FFileHelper::LoadFileToString(jsonString, *JsonFilePath);

	TSharedPtr<FJsonValue> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);

	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		auto aItems = jsonObject->AsArray();
		for (auto item : aItems)
		{
			FItemStack stack;
			stack.Item.Name = item->AsObject()->GetStringField("Name");
			stack.Amount = 0;
			mItemList.Add(stack);
		}
	}
}

void APlayerCharacter::MoveForward(float inputVal)
{
	FVector forward = UKismetMathLibrary::GetForwardVector(GetControlRotation());
	if (mIsControlling)
	{
		if (inputVal == 0.f || mpSelectedMovementComponent->IsMoving())
			return;

		forward *= inputVal;

		MoveObject(forward);
	}
	else
		AddMovementInput(forward, inputVal);
}

void APlayerCharacter::MoveRight(float inputVal)
{
	FVector right = UKismetMathLibrary::GetRightVector(GetControlRotation());
	if (mIsControlling)
	{
		if (inputVal == 0.f || mpSelectedMovementComponent->IsMoving())
			return;

		right *= inputVal;
		MoveObject(right);
	}
	else
		AddMovementInput(right, inputVal);
}

void APlayerCharacter::Control()
{
	if (mpSelectedMovementComponent)
	{
		mIsControlling = !mIsControlling;
		USceneComponent* rootComp = mpSelectedMovementComponent->GetOwner()->GetRootComponent();
		if (rootComp)
		{
			FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
			mpSpringArm->bDoCollisionTest = false;
			mpSpringArm->AttachToComponent(rootComp, transformRules);
		}
		Deselect();
	}
}

void APlayerCharacter::UnControl()
{
	if (mpSelectedMovementComponent)
	{
		mIsControlling = !mIsControlling;
		FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
		mpSpringArm->bDoCollisionTest = true;
		mpSpringArm->AttachToComponent(RootComponent, transformRules);
	}
}

void APlayerCharacter::RotateObject()
{
	mpSelectedMovementComponent->Rotate();
}

void APlayerCharacter::Push()
{
	if (mpSelectedMinecartMovement)
	{
		mpSelectedMinecartMovement->Move(GetActorForwardVector());
	}
}

void APlayerCharacter::SwitchSeason()
{
	switch (mCurrentSeason)
	{
	case Season::SPRING:
		mCurrentSeason = Season::SUMMER;
		mpSeasonParticleEmitter->Deactivate();
		break;
	case Season::SUMMER:
		mCurrentSeason = Season::FALL;
		mpSeasonParticleEmitter->SetMaterial(0, mpLeafFall_Mat);
		mpSeasonParticleEmitter->Activate();
		break;
	case Season::FALL:
		mCurrentSeason = Season::WINTER;
		mpSeasonParticleEmitter->SetMaterial(0, mpSnowflake_Mat);
		break;
	case Season::WINTER:
		mCurrentSeason = Season::SPRING;
		mpSeasonParticleEmitter->SetMaterial(0, mpLeafSpring_Mat);
		break;
	default:
		break;
	}
}

void APlayerCharacter::Interact()
{
	if (mpClosestTree)
		mpClosestTree->CollectResource(this);
}

void APlayerCharacter::AddItem(const FString& name, const int& amount)
{
	auto item = mItemList.FindByPredicate([name](FItemStack stack)
		{
			return stack.Item.Name.Equals(name);
		});

	item->Amount += amount;
}

