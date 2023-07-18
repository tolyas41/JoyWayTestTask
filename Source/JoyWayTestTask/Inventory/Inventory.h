// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums.h"
#include "Structs.h"

#include "Inventory.generated.h"

class AItem;
class AJoyWayTestTaskCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChanged, int32, CurrentAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemsUpdated);

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCommonItem ItemStats;
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponItem WeaponStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo;
};

UCLASS(Blueprintable)
class JOYWAYTESTTASK_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventory();

protected:
	virtual void BeginPlay() override;


public:	
	UPROPERTY(BlueprintReadWrite)
	TArray<FItemInfo> Items;

	UPROPERTY(BlueprintReadWrite)
	TArray<FWeaponInfo> Weapons;

	UPROPERTY(BlueprintReadOnly)
	FName CurrentWeapon;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentWeaponIndex = -1;

	void AddItem(TWeakObjectPtr<AItem> newItem);
	void AddWeapon(TWeakObjectPtr<AItem> newWeapon);

	void SelectNextWeapon();
	void SelectPreviousWeapon();
	bool ReduceAmmo(int32 ByCount);
	void ReloadCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo() const;

	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FAmmoChanged OnAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category = "Items")
	FItemsUpdated OnItemsUpdated;
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateItems();
	UFUNCTION(BlueprintCallable)
	void UpdateWeapon();

private:
	void SetWeapon(FWeaponInfo Weapon);

	TWeakObjectPtr<AJoyWayTestTaskCharacter> OwnerCharacter;
};
