// Fill out your copyright notice in the Description page of Project Settings.


#include "Rail.h"
#include "ProjectS21/Grid/CellPoint.h"
#include "ProjectS21/Minecart/Minecart.h"
#include "GameFramework/FloatingPawnMovement.h"

ARail::ARail()
{
	const ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/ProjectS21Assets/Rails/SM_Rail"));
	mpMesh->SetStaticMesh(mesh.Object);

	UArrowComponent* newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward"));
	FVector forward = GetActorForwardVector();
	newArrow->SetRelativeRotation(forward.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Backward"));
	forward = -forward;
	newArrow->SetRelativeRotation(forward.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	UFloatingPawnMovement* movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
	UGridMovement* gridMovement = CreateDefaultSubobject<UGridMovement>(TEXT("GridMovementComponent"));
	gridMovement->SetMovementComponent(movement);
	AddOwnedComponent(movement);
	AddOwnedComponent(gridMovement);

	mType = Type::STRAIGHT;
}

void ARail::BeginPlay()
{
	Super::BeginPlay();

	FSplinePoint start, end;
	FVector forward, backward;
	forward = GetActorForwardVector();
	backward = -forward;

	SetSplinePoint(0, backward);
	SetSplinePoint(1, forward);
}

void ARail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ARail::GetNextDirection(const FVector& direction)
{
	auto arrow = maDirections.FindByPredicate([direction](UArrowComponent* comp)
		{
			FVector forward = comp->GetForwardVector();
			return forward.Equals(direction);
		});
	return (*arrow)->GetForwardVector();
}

USplineComponent* ARail::GetTrack(const FVector& direction)
{
	FVector startPoint;
	startPoint = GetActorLocation() - (direction * 100.f);
	for (size_t i = 0; i < mpSpline->GetNumberOfSplinePoints(); i++)
	{
		FVector location = mpSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		if (location.Equals(startPoint, 0.01f))
		{
			if (i == 0)
				return mpSpline;
			else
			{
				FVector temp = mpSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
				mpSpline->SetLocationAtSplinePoint(0, mpSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local), ESplineCoordinateSpace::Local);
				mpSpline->SetLocationAtSplinePoint(1, temp, ESplineCoordinateSpace::Local);
				return mpSpline;
			}
		}
	}
	return nullptr;
}
