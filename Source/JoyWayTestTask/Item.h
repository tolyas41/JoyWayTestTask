// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Enums.h"

#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;


UCLASS()
class JOYWAYTESTTASK_API AItem : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* PickUpCollision;

public:	
	AItem();

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings, meta = (ExposeOnSpawn = true))
	FDataTableRowHandle ItemID;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EItemType> ItemType;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void RunConstruction_BP();

private:
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void LoadItemMesh();
};
