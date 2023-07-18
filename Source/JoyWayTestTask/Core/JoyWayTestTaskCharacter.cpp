// Copyright Epic Games, Inc. All Rights Reserved.

#include "JoyWayTestTaskCharacter.h"
#include "JoyWayTestTaskProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AJoyWayTestTaskCharacter

AJoyWayTestTaskCharacter::AJoyWayTestTaskCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	Gun->bCastDynamicShadow = false;
	Gun->CastShadow = false;
	//Gun->SetupAttachment(Mesh1P);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
}

void AJoyWayTestTaskCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));	

}

//////////////////////////////////////////////////////////////////////////
// Input

void AJoyWayTestTaskCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, Inventory, &UInventory::SelectNextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Released, Inventory, &UInventory::SelectPreviousWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Released, Inventory, &UInventory::ReloadCurrentWeapon);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJoyWayTestTaskCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AJoyWayTestTaskCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJoyWayTestTaskCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJoyWayTestTaskCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJoyWayTestTaskCharacter::LookUpAtRate);
}

void AJoyWayTestTaskCharacter::SetNewWeapon(FWeaponItem WeaponStats, UStaticMesh* WeaponMesh)
{
	CurrentWeaponStats = WeaponStats;
	Gun->SetStaticMesh(WeaponMesh);
}

void AJoyWayTestTaskCharacter::OnFire()
{
	if (Gun->GetStaticMesh() != nullptr && Inventory->ReduceAmmo(1))
	{
		FHitResult Hit;
		FVector Start = FirstPersonCameraComponent->GetComponentLocation(); 
		FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * 5000.f;
		FCollisionQueryParams TraceParams;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		WeaponImpact(Hit);
		if (Hit.IsValidBlockingHit())
		{
			TSubclassOf<UDamageType> DamageType;
			UGameplayStatics::ApplyDamage(Hit.GetActor(), CurrentWeaponStats.Damage, GetController(), this, DamageType);
		}
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			TWeakObjectPtr<UAnimInstance> AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance.IsValid())
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}


void AJoyWayTestTaskCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AJoyWayTestTaskCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AJoyWayTestTaskCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJoyWayTestTaskCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
