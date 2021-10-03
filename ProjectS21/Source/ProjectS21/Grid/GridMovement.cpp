// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMovement.h"
#include "CellPoint.h"

// Sets default values for this component's properties
UGridMovement::UGridMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGridMovement::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGridMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mpPawnMovementComp
		&& mpNextCell
		&& mIsMoving
		&& !mIsRotating)
	{
		UpdateMovement();
	}
	else if (mpPawnMovementComp
		&& !mIsMoving
		&& mIsRotating)
	{
		UpdateRotation();
	}
	else
	{
		mIsMoving = false;
		mIsRotating = false;
	}
}

void UGridMovement::SetMovementComponent(UPawnMovementComponent* pawnMovement)
{
	mpPawnMovementComp = pawnMovement;
}

void UGridMovement::Move(ACellPoint* nextCell, const FVector& direction)
{
	if (!mIsMoving)
	{
		mpNextCell = nextCell;
		mDirection = direction;
		mIsMoving = !mIsMoving;
	}
}

void UGridMovement::Rotate()
{
	if (!mIsRotating)
		mIsRotating = !mIsRotating;
}

bool UGridMovement::HasReachedDestination()
{
	FVector ownerLocation = GetOwner()->GetActorLocation();
	FVector destination = mpNextCell->GetActorLocation();

	//get location on the axis
	ownerLocation *= mDirection.GetAbs();
	destination *= mDirection.GetAbs();

	FVector distance = destination - ownerLocation;
	distance.Normalize();

	if (!distance.Equals(mDirection))
		return true;
	return false;
}

void UGridMovement::UpdateMovement()
{
	//if not reached destination
	if (!HasReachedDestination())
	{
		//add input
		mpPawnMovementComp->AddInputVector(mDirection);
		return;

	}
	auto owner = GetOwner();
	mpPawnMovementComp->Velocity = {};
	//owner->SetActorLocation(mpNextCell->GetActorLocation(), false, nullptr, ETeleportType::ResetPhysics);
	owner->AttachToActor(mpNextCell, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
	mIsMoving = false;
}

void UGridMovement::UpdateRotation()
{
	float rotation = mRotationRate / 360.f;
	mCountYawRotation += rotation;

	FRotator addRotation;
	FRotator newRotation;
	if (mCountYawRotation >= 90.f)
	{
		//get last piece of rotation to get a full 90 degree turn
		float lastRotation = rotation - (mCountYawRotation - 90.f);
		addRotation = FRotator(0, lastRotation, 0);
		mIsRotating = !mIsRotating;
		mCountYawRotation = 0.f;
	}
	else
		addRotation = FRotator(0, rotation, 0);
	newRotation = GetOwner()->GetActorRotation();
	newRotation += addRotation;
	GetOwner()->SetActorRotation(newRotation);
}

