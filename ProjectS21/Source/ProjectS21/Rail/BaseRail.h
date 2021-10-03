// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectS21/Grid/GridMovement.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Components\WidgetComponent.h"
#include "BaseRail.generated.h"

enum class Type
{
	STRAIGHT,
	CURVE,
	CROSS
};

UENUM()
enum RepairMaterials
{
	Wood,
	Acorn,
	Maple,
	Cherry
};

UCLASS(BlueprintType)
class PROJECTS21_API ABaseRail : public APawn
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		UWidgetComponent* mpRepairWidget;

public:
	// Sets default values for this pawn's properties
	ABaseRail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetSplinePoint(const int& indx, const FVector& direction, const float& tangentMultiplier = 1.0f);
	void AddSplinePoint(const FVector& direction, const float& tangentMultiplier = 1.0f);

protected:
	UStaticMeshComponent* mpMesh;
	TArray<UArrowComponent*> maDirections;
	UPROPERTY(VisibleAnywhere)
		USplineComponent* mpSpline;
	float mHalfRailOffset = 100.f;
	FVector mCurrentInputCartDirection = { 0,0,0 };

	UPROPERTY(EditAnywhere, Category="Repair")
		bool mNeedRepair = false;

	UPROPERTY(EditAnywhere, Category = "Repair")
		TMap<TEnumAsByte<RepairMaterials>, int> maRepairMaterials;
	Type mType;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	Type GetType() { return mType; }
	bool HasDirection(const FVector& direction);
	FVector GetCurrentInputCartDirection() { return mCurrentInputCartDirection; }
	void SetCurrentInputCartDirection(const FVector& currentInputDirection) { mCurrentInputCartDirection = currentInputDirection; }
	virtual FVector GetNextDirection(const FVector& direction);
	virtual USplineComponent* GetTrack(const FVector& direction) { return nullptr; }

	UFUNCTION(BlueprintCallable, Category="Actor")
		TMap<TEnumAsByte<RepairMaterials>, int> GetRepairMaterialsList() const { return maRepairMaterials; }
	void ShowWidget(bool visibility);
};
