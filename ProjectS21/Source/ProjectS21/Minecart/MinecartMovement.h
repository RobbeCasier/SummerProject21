// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectS21/Rail/BaseRail.h"
#include "Minecart.h"
#include "MinecartMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTS21_API UMinecartMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMinecartMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Move(const FVector& direction);
	bool IsMoving() { return mIsMoving; }

private:
	bool IsValidMovement(const FVector& destination);
	bool HasReachedDestination();
	void UpdateMovement();
private:
	UPawnMovementComponent* mpMovement;

	ABaseRail* mpNextRail;
	AMinecart* mpOwner;
	FVector mDirection;

	float mTrackAlpha = 0.5f;
	bool mIsMoving = false;
};
