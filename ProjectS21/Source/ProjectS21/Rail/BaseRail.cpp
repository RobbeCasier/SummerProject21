// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRail.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ABaseRail::ABaseRail()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* pSceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = pSceneComp;

	mpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mpMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	mpMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	mpMesh->SetupAttachment(pSceneComp);

	mpSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Tracks"));
	mpSpline->SetupAttachment(pSceneComp);
	
	mpRepairWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	mpRepairWidget->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UUserWidget> interactWidget(TEXT("/Game/ProjectS21/Widgets/BP_RepairInteract"));
	auto widgetClass = interactWidget.Class;
	mpRepairWidget->SetWidgetClass(widgetClass);
	mpRepairWidget->SetWidgetSpace(EWidgetSpace::Screen);
	mpRepairWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void ABaseRail::BeginPlay()
{
	Super::BeginPlay();
	mpRepairWidget->SetDrawSize({ 500,300 });
}

void ABaseRail::SetSplinePoint(const int& indx, const FVector& direction, const float& tangentMultiplier)
{
	FVector location = GetActorLocation();
	mpSpline->SetLocationAtSplinePoint(indx, location + (direction * mHalfRailOffset), ESplineCoordinateSpace::World);
	mpSpline->SetTangentsAtSplinePoint(indx, direction * mHalfRailOffset * tangentMultiplier, -direction * mHalfRailOffset * tangentMultiplier, ESplineCoordinateSpace::Type::World);
}

void ABaseRail::AddSplinePoint(const FVector& direction, const float& tangentMultiplier)
{
	FVector location = GetActorLocation();
	mpSpline->AddSplineWorldPoint(location + (direction * mHalfRailOffset));
	int lastIndx = mpSpline->GetNumberOfSplinePoints()-1;
	mpSpline->SetTangentsAtSplinePoint(lastIndx, direction * mHalfRailOffset * tangentMultiplier, -direction * mHalfRailOffset * tangentMultiplier, ESplineCoordinateSpace::World);
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

bool ABaseRail::HasDirection(const FVector& direction)
{
	auto arrow = maDirections.FindByPredicate([direction](UArrowComponent* comp)
		{
			FVector forward = comp->GetForwardVector();
			return forward.Equals(direction);
		});
	if (arrow)
		return true;
	return false;
}

//Get the next direction, always return a direction
FVector ABaseRail::GetNextDirection(const FVector& direction)
{
	return FVector{};
}

void ABaseRail::ShowWidget(bool visibility)
{
	if (mNeedRepair)
		mpRepairWidget->SetVisibility(visibility);
}

