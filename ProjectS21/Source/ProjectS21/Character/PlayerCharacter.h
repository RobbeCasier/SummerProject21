// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectS21/Minecart/MinecartMovement.h"
//#include "Materials/MaterialInterface.h"
#include "ProjectS21/Grid/GridMovement.h"
#include "ProjectS21/Season.h"
#include "ProjectS21/Items/Item.h"
#include "PlayerCharacter.generated.h"

class ATree;

UCLASS()
class PROJECTS21_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float inputVal);
	void MoveRight(float inputVal);
	void Control();
	void UnControl();
	void RotateObject();
	void Push();
	void SwitchSeason();
	void Interact();
	void AddItem(const FString& name, const int& amount);

	UFUNCTION(BlueprintCallable, Category="Character")
	TArray<FItemStack> GetItems() const { return mItemList; }
	UFUNCTION(BlueprintCallable, Category="Character")
		ABaseRail* GetBrokenRail() const { return mpBrokenRail; }
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* mpSpringArm;
	UPROPERTY(EditAnywhere)
		UCameraComponent* mpCamera;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* mpSeasonParticleEmitter;

private:
	void CheckForMovableObject();
	void CheckForMinecart();
	void FindClosestTree();
	void MoveObject(const FVector& direction);
	void Deselect();
	void UpdateEnvironment();
	void InitItems();

private:
	UGridMovement* mpSelectedMovementComponent;
	UMinecartMovement* mpSelectedMinecartMovement;
	UParticleSystem* mpParticleSystem;

	UMaterialInstance* mpLeafFall_Mat;
	UMaterialInstance* mpLeafSpring_Mat;
	UMaterialInstance* mpSnowflake_Mat;
	ATree* mpClosestTree;
	ABaseRail* mpBrokenRail;

	TArray<FItemStack> mItemList;

	FVector mParticleRelativeLocation = { 0,0,200 };
	float mSeasonRadius = 250.f;
	Season mCurrentSeason = Season::SPRING;
	bool mIsControlling = false;
};
