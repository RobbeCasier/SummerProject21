// Fill out your copyright notice in the Description page of Project Settings.


#include "MinecartMovement.h"
#include "MinecartMovement.h"
#include "ProjectS21/Rail/BaseRail.h"
#include "ProjectS21/Grid/CellPoint.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UMinecartMovement::UMinecartMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UMinecartMovement::BeginPlay()
{
	Super::BeginPlay();

	auto pOwner = Cast<APawn>(GetOwner());
	if (pOwner)
	{
		mpOwner = Cast<AMinecart>(pOwner);
	}
}


// Called every frame
void UMinecartMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mIsMoving)
		UpdateMovement();
}

void UMinecartMovement::Move(const FVector& direction)
{
	if (!mpOwner || mIsMoving)
		return;

	if (IsValidMovement(direction))
	{
		mIsMoving = true;
	}
}

bool UMinecartMovement::IsValidMovement(const FVector& direction)
{
	//check backward, because of curve
	//first point is always at this location
	ABaseRail* pRail = Cast<ABaseRail>(mpOwner->GetAttachParentActor());
	if (!pRail->HasDirection(-direction))
		return false;

	//get parent
	auto attachParent = pRail->GetAttachParentActor();
	ACellPoint* attachCell = (attachParent) ? Cast<ACellPoint>(attachParent) : nullptr;

	mDirection = pRail->GetNextDirection(direction);
	if (pRail->GetType() == Type::CROSS)
	{
		FVector cartDir = pRail->GetCurrentInputCartDirection();
		if (!cartDir.Equals(mDirection))
			if (!cartDir.Equals(-mDirection))
				return false;
	}
	//find the neighbour
	ACellPoint* nextCell = attachCell->GetNeighbour(mDirection);
	if (nextCell)
	{
		//see if neighbour has a rail
		TArray<AActor*> attachedActors;
		nextCell->GetAttachedActors(attachedActors);
		if (attachedActors.Num() == 0)
			return false;
		AActor* nextRailActor = *attachedActors.FindByPredicate([](AActor* actor)
			{
				return Cast<ABaseRail>(actor);
			});

		if (nextRailActor)
		{
			//see if this rails path is possible to drive to
			ABaseRail* nextRail = Cast<ABaseRail>(nextRailActor);

			if (!nextRail->HasDirection(-mDirection))
				return false;

			nextRail->SetCurrentInputCartDirection(direction);
			mpNextRail = nextRail;
			return true;
		}
	}
	return false;
}

bool UMinecartMovement::HasReachedDestination()
{
	FVector ownerLocation = GetOwner()->GetActorLocation();
	FVector destination = mpNextRail->GetActorLocation();

	//get location on the axis
	ownerLocation *= mDirection.GetAbs();
	destination *= mDirection.GetAbs();

	FVector distance = destination - ownerLocation;
	distance.Normalize();

	if (!distance.Equals(mDirection))
		return true;
	return false;
}

void UMinecartMovement::UpdateMovement()
{
	mTrackAlpha += GetWorld()->DeltaTimeSeconds;
	USplineComponent* pTrack;
	if (!mpNextRail && mTrackAlpha >= 0.5f)
	{
		mTrackAlpha = 0.5f;
		mIsMoving = false;
	}
	else if (mTrackAlpha >= 1.f)
	{
		mTrackAlpha = mTrackAlpha - 1.f;

		auto owner = GetOwner();
		owner->AttachToActor(mpNextRail, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
		mpOwner->SetRail(mpNextRail);

		if (!IsValidMovement(mDirection))
			mpNextRail = nullptr;
	}

	auto ownerLocation = mpOwner->GetRail()->GetActorLocation();
	float halfRail = 100.f;
	pTrack = mpOwner->GetRail()->GetTrack(mDirection);

	if (!pTrack)
	{
		mIsMoving = false;
		return;
	}
	float length = pTrack->GetSplineLength();
	float distanceOnTrack = FMath::Lerp(0.f, length, mTrackAlpha);
	FVector location = pTrack->GetLocationAtDistanceAlongSpline(distanceOnTrack, ESplineCoordinateSpace::Type::World);
	FRotator rotation = pTrack->GetRotationAtDistanceAlongSpline(distanceOnTrack, ESplineCoordinateSpace::Type::World);

	mpOwner->SetActorLocationAndRotation(location, rotation);
}


