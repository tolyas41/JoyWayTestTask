// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Item.h"
#include "JoyWayTestTaskCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventory, All, All);

UInventory::UInventory()
{

}

void UInventory::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AJoyWayTestTaskCharacter>(GetOwner());
	if (!OwnerCharacter.IsValid())
	{
		UE_LOG(LogInventory, Error, TEXT("Inventory owner is not valid!"));
		return;
	}
	UpdateWeapon();
}

void UInventory::AddItem(TWeakObjectPtr<AItem> newItem)
{
	if (!newItem.IsValid())
	{
		UE_LOG(LogInventory, Error, TEXT("Try to add an invalid Item!"));
		return;
	}
	FItemInfo NewItem;
	NewItem.Name = newItem.Get()->ItemID.RowName;
	NewItem.ItemStats = *newItem.Get()->ItemID.DataTable->FindRow<FCommonItem>(NewItem.Name, "CommonItem data");
	
	Items.Add(NewItem);
	OnItemsUpdated.Broadcast();
}

void UInventory::AddWeapon(TWeakObjectPtr<AItem> newWeapon)
{
	if (!newWeapon.IsValid())
	{
		UE_LOG(LogInventory, Error, TEXT("Try to add an invalid Weapon!"));
		return;
	}
	FWeaponInfo NewWeapon;
	NewWeapon.Name = newWeapon->ItemID.RowName;
	NewWeapon.WeaponStats = *newWeapon->ItemID.DataTable->FindRow<FWeaponItem>(NewWeapon.Name, "WeaponItem data");
	NewWeapon.WeaponMesh = newWeapon->StaticMesh->GetStaticMesh();
	NewWeapon.CurrentAmmo = NewWeapon.WeaponStats.ClipSize;
	int32 WeaponIndex = Weapons.Add(NewWeapon);
	if (CurrentWeaponIndex == -1)
	{
		CurrentWeaponIndex = WeaponIndex;
		UpdateWeapon();
	}
	OnItemsUpdated.Broadcast();
}

void UInventory::SelectNextWeapon()
{
	if (Weapons.IsValidIndex(1))
	{
		if (Weapons.IsValidIndex(CurrentWeaponIndex + 1))
		{
			CurrentWeaponIndex++;
		}
		else
		{
			CurrentWeaponIndex = 0;
		}
		SetWeapon(Weapons[CurrentWeaponIndex]);
	}
}

void UInventory::SelectPreviousWeapon()
{
	if (Weapons.IsValidIndex(1))
	{
		if (Weapons.IsValidIndex(1))
		{
			if (Weapons.IsValidIndex(CurrentWeaponIndex - 1))
			{
				CurrentWeaponIndex--;
			}
			else
			{
				CurrentWeaponIndex = Weapons.Num() - 1;
			}
			SetWeapon(Weapons[CurrentWeaponIndex]);
		}
	}
}

bool UInventory::ReduceAmmo(int32 ByCount)
{
	if (Weapons[CurrentWeaponIndex].CurrentAmmo >= ByCount)
	{
		Weapons[CurrentWeaponIndex].CurrentAmmo -= ByCount;
		OnAmmoChanged.Broadcast(Weapons[CurrentWeaponIndex].CurrentAmmo);
		return true;
	}
	else
	{
		return false;
	}
}

int32 UInventory::GetCurrentAmmo() const
{
	return Weapons.IsValidIndex(CurrentWeaponIndex) ? Weapons[CurrentWeaponIndex].CurrentAmmo : 0;
}

void UInventory::ReloadCurrentWeapon()
{
	if (Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		Weapons[CurrentWeaponIndex].CurrentAmmo = Weapons[CurrentWeaponIndex].WeaponStats.ClipSize;
		OnAmmoChanged.Broadcast(Weapons[CurrentWeaponIndex].CurrentAmmo);
	}
}

void UInventory::UpdateWeapon()
{
	if (CurrentWeapon == FName("None") && CurrentWeaponIndex >= 0 && Weapons.IsValidIndex(CurrentWeaponIndex))
	{
		SetWeapon(Weapons[CurrentWeaponIndex]);
		OnAmmoChanged.Broadcast(Weapons[CurrentWeaponIndex].CurrentAmmo);
	}
}

void UInventory::SetWeapon(FWeaponInfo Weapon)
{
	CurrentWeapon = Weapon.Name;
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetNewWeapon(Weapon.WeaponStats, Weapon.WeaponMesh);
		OnAmmoChanged.Broadcast(Weapons[CurrentWeaponIndex].CurrentAmmo);
	}
	
}


