// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Inventory.h"

AItem::AItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMesh;

	PickUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollision"));
	PickUpCollision->InitSphereRadius(20.f);
	PickUpCollision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnCollisionBeginOverlap);
	PickUpCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpCollision->SetupAttachment(StaticMesh);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (!StaticMesh->GetStaticMesh())
	{
		Destroy(); //Destroy item if static mesh load fails = invalid DT/RowName/StaticMesh
	}
}

void AItem::RunConstruction_BP()
{
	if (ItemID.DataTable)
	{
		if (ItemID.DataTable->GetFName() == FName("DT_CommonItems"))
		{
			ItemType = Common;
		}
		if (ItemID.DataTable->GetFName() == FName("DT_Weapons"))
		{
			ItemType = Weapon;
		}
	}
	LoadItemMesh();
}

void AItem::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::White, FString::Printf(TEXT("%s "), *OtherActor->GetName()));
	if (Cast<ACharacter>(OtherActor))
	{
		UInventory* InventoryComponent = Cast<UInventory>(OtherActor->GetComponentByClass(UInventory::StaticClass()));
		if (ItemType == Common)
		{
			InventoryComponent->AddItem(this);
		}
		if (ItemType == Weapon)
		{
			InventoryComponent->AddWeapon(this);
		}
		Destroy();
	}
}

void AItem::LoadItemMesh()
{
	FString Path;
	switch (ItemType)
	{
	case Common:
		Path = "/Game/Blueprints/Items/StaticMeshes/Common/";
		break;
	case Weapon:
		Path = "/Game/Blueprints/Items/StaticMeshes/Weapons/";
		break;
	}
	Path = Path + ItemID.RowName.ToString() + "." + ItemID.RowName.ToString();
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, *Path);
	if (!Mesh)
	{
		StaticMesh->SetStaticMesh(nullptr);
		return;
	}

	StaticMesh->SetStaticMesh(Mesh);
}

