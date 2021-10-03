// Fill out your copyright notice in the Description page of Project Settings.


#include "CellPoint.h"

// Sets default values
ACellPoint::ACellPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mpLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Location"));
	RootComponent = mpLocationComponent;

	mpSelectionPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionPlane"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> plane(TEXT("/Engine/BasicShapes/Plane"));
	mpSelectionPlane->SetStaticMesh(plane.Object);
	mpSelectionPlane->SetupAttachment(RootComponent);
	mpSelectionPlane->SetRelativeScale3D({ 2,2,2 });
	mpSelectionPlane->SetRelativeLocation({ 0,0,1 });
	mpSelectionPlane->SetVisibility(false);

	const ConstructorHelpers::FObjectFinder<UMaterialInstance> selection(TEXT("/Game/ProjectS21Assets/MI_GridSelection"));
	mpSelectionMat = CreateDefaultSubobject<UMaterialInstance>(TEXT("SelectionMaterial"));
	mpSelectionMat = selection.Object;

	mpSelectionPlane->SetMaterial(0, mpSelectionMat);
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
	if (mpSpawnClass != mpPreviousSpawnClass && mpSpawnClass != nullptr)
	{
		const FVector location = GetActorLocation();
		auto actor = GetWorld()->SpawnActor(mpSpawnClass, &location);
		actor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		mpPreviousSpawnClass = mpSpawnClass;
	}
	else
		mpPreviousSpawnClass = nullptr;
}

void ACellPoint::Select()
{
	mpSelectionPlane->SetVisibility(true);
}

void ACellPoint::Deselect()
{
	mpSelectionPlane->SetVisibility(false);
}

ACellPoint* ACellPoint::GetNeighbour(const FVector& direction)
{
	//this instead of contains, because direction can have -0 instead of 0 and contain sees this as a different value
	TArray<FVector> keys;
	maNeighbours.GetKeys(keys);
	FVector* key = keys.FindByPredicate([direction](FVector dir)
		{
			return dir.Equals(direction);
		});
	if (key)
		return maNeighbours[*key];
	return nullptr;
}

