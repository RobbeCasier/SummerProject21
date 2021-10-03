// Fill out your copyright notice in the Description page of Project Settings.


#include "CurveRailStatic.h"

ACurveRailStatic::ACurveRailStatic()
{
	const ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("/Game/ProjectS21Assets/Rails/SM_CurveRail_Static"));
	mpMesh->SetStaticMesh(mesh.Object);

	UArrowComponent* newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right"));
	FVector right = GetActorRightVector();
	newArrow->SetRelativeRotation(right.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	newArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Backward"));
	FVector backward = -GetActorForwardVector();
	newArrow->SetRelativeRotation(backward.Rotation());
	maDirections.Add(newArrow);
	newArrow->SetupAttachment(RootComponent);

	mType = Type::CURVE;
}

void ACurveRailStatic::BeginPlay()
{
	Super::BeginPlay();

	FSplinePoint start, end;
	FVector right, backward;
	right = GetActorRightVector();
	backward = -GetActorForwardVector();

	SetSplinePoint(0, backward, 2.f);
	SetSplinePoint(1, right, 2.f);
}

void ACurveRailStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ACurveRailStatic::GetNextDirection(const FVector& direction)
{
	auto arrow = maDirections.FindByPredicate([direction](UArrowComponent* comp)
		{
			FVector forward = comp->GetForwardVector();
			if (!forward.Equals(-direction))
				return true;
			return false;
		});
	return (*arrow)->GetForwardVector();
}

USplineComponent* ACurveRailStatic::GetTrack(const FVector& direction)
{
	//last point connects to ther rail
	FVector lastPoint;
	lastPoint = GetActorLocation() + (direction * 100.f);
	for (size_t i = 0; i < mpSpline->GetNumberOfSplinePoints(); i++)
	{
		FVector location = mpSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		if (location.Equals(lastPoint, 0.01f))
		{
			if (i != 0)
				return mpSpline;
			else
			{
				FVector temp = mpSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
				FVector tempTangent = mpSpline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Type::Local);
				FVector tangent = mpSpline->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::Local);
				mpSpline->SetLocationAtSplinePoint(0, mpSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local), ESplineCoordinateSpace::Local);
				mpSpline->SetTangentsAtSplinePoint(0, -tangent, tangent, ESplineCoordinateSpace::Local);
				mpSpline->SetLocationAtSplinePoint(1, temp, ESplineCoordinateSpace::Local);
				mpSpline->SetTangentsAtSplinePoint(1, -tempTangent, tempTangent, ESplineCoordinateSpace::Local);
				return mpSpline;
			}
		}
	}
	return nullptr;
}
