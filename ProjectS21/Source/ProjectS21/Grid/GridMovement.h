// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CellPoint.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GridMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTS21_API UGridMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMovementComponent(UPawnMovementComponent* pawnMovement);
	void Move(ACellPoint* nextCell, const FVector& direction);
	bool IsMoving() { return mIsMoving; }

	void Rotate();
private:
	bool HasReachedDestination();
	void UpdateMovement();
	void UpdateRotation();
private:
	UPawnMovementComponent* mpPawnMovementComp;

	ACellPoint* mpNextCell;
	FVector mDirection;
	float mRotationRate = 1000.f;
	float mCountYawRotation = 0.f;
	bool mIsMoving = false;
	bool mIsRotating = false;
};
