// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Structs.generated.h"

USTRUCT(BlueprintType)
struct FCommonItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
};

USTRUCT(BlueprintType)
struct FWeaponItem : public FCommonItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ClipSize;
};
