// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly)
	FString Name;
};

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly)
	FItem Item;
		UPROPERTY(BlueprintReadOnly)
	int Amount;
};
