// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossRailStatic.h"

ACrossRailStatic::ACrossRailStatic()
{
	const ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/ProjectS21Assets/Rails/SM_CrossRail_Static"));
	mpMesh->SetStaticMesh(mesh.Object);

	UArrowComponent* newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward"));
	FVector forward = GetActorForwardVector();
	newArrow->SetRelativeRotation(forward.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Backward"));
	FVector backward = -forward;
	newArrow->SetRelativeRotation(backward.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right"));
	FVector right = GetActorRightVector();
	newArrow->SetRelativeRotation(right.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left"));
	FVector left = -right;
	newArrow->SetRelativeRotation(left.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	mType = Type::CROSS;
}

void ACrossRailStatic::BeginPlay()
{
	Super::BeginPlay();

	FVector right, forward;
	right = GetActorRightVector();
	forward = GetActorForwardVector();

	//order
	//[0] TrackAStart
	SetSplinePoint(0, -forward);
	//[1] TrackBStart
	SetSplinePoint(1, -right);
	//[2] TrackAEnd
	AddSplinePoint(forward);
	//[3] TrackBEnd
	AddSplinePoint(right);
}

void ACrossRailStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ACrossRailStatic::GetNextDirection(const FVector& direction)
{
	auto arrow = maDirections.FindByPredicate([direction](UArrowComponent* comp)
		{
			FVector forward = comp->GetForwardVector();
			return forward.Equals(direction);
		});
	return (*arrow)->GetForwardVector();
}

USplineComponent* ACrossRailStatic::GetTrack(const FVector& direction)
{
	//last point connects to ther rail
	FVector firstPoint;
	firstPoint = GetActorLocation() + (-direction * 100.f);
	for (size_t i = 0; i < mpSpline->GetNumberOfSplinePoints(); i++)
	{
		FVector location = mpSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		if (location.Equals(firstPoint, 0.01f))
		{
			int nextInd = (i + 2) % mpSpline->GetNumberOfSplinePoints();
			USplineComponent* pTrack = NewObject<USplineComponent>();
			pTrack->SetWorldLocation(GetActorLocation());
			pTrack->SetLocationAtSplinePoint(0, mpSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local), ESplineCoordinateSpace::Local);
			pTrack->SetLocationAtSplinePoint(1, mpSpline->GetLocationAtSplinePoint(nextInd, ESplineCoordinateSpace::Local), ESplineCoordinateSpace::Local);
			return pTrack;
		}
	}
	return nullptr;
}

