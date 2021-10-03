// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"
#include "ProjectS21/Character/PlayerCharacter.h"

// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mpTreeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree"));
	mpTreeMeshComponent->SetWorldScale3D({ 0.5,0.5,0.5 });
	mpTreeMeshComponent->SetStaticMesh(mpTree);

	mpInteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	mpInteractWidget->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UUserWidget> interactWidget(TEXT("/Game/ProjectS21/Widgets/BP_Interact"));
	auto widgetClass = interactWidget.Class;
	mpInteractWidget->SetWidgetClass(widgetClass);
	mpInteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	mpInteractWidget->SetVisibility(false);
}

ATree::~ATree()
{
}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
	mpInteractWidget->SetRelativeLocation({ 0,0,200 });
	mpInteractWidget->SetPivot({ 0.1,0 });
	mpInteractWidget->SetDrawSize({ 400, 100 });
	
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATree::ChangeSeason(const Season& newSeason)
{
	mCurrentSeason = newSeason;
	switch (mCurrentSeason)
	{
	case Season::SPRING:
		mpTreeMeshComponent->SetStaticMesh(mpTree);
		break;
	case Season::SUMMER:
		mpTreeMeshComponent->SetStaticMesh(mpTree);
		break;
	case Season::FALL:
		mpTreeMeshComponent->SetStaticMesh(mpFallTree);
		break;
	case Season::WINTER:
		break;
	default:
		break;
	}
}

void ATree::CollectResource(APlayerCharacter* pPlayer)
{
	if (mHasCollectedResource)
		return;

	ShowInteractionWidget(false);
	mHasCollectedResource = true;
	switch (mCurrentSeason)
	{
	case Season::SPRING:
		pPlayer->AddItem("Maple", mAmountOfMaple);
		break;
	case Season::SUMMER:
		pPlayer->AddItem("Cherry", mAmountOfCherries);
		break;
	case Season::FALL:
		pPlayer->AddItem("Acorn", mAmountOfAcorn);
		break;
	case Season::WINTER:
		pPlayer->AddItem("Wood", mAmountOfWood);
		break;
	default:
		break;
	}
}

void ATree::ShowInteractionWidget(bool visibility)
{
	if (!mHasCollectedResource)
		mpInteractWidget->SetVisibility(visibility);
}

