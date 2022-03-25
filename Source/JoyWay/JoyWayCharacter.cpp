// Copyright Epic Games, Inc. All Rights Reserved.

#include "JoyWayCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "MotionControllerComponent.h"
#include "WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AJoyWayCharacter

AJoyWayCharacter::AJoyWayCharacter()
{
	bUseControllerRotationYaw = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.f, 100.f);

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	WalkSpeeds.Add(EWalkMode::Walk, 200.f);
	WalkSpeeds.Add(EWalkMode::Sprint, 600.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetOffset = FVector(0, 0, 100.f);
	CameraBoom->SetRelativeLocation(FVector(0, 0, 30));
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->SetRelativeLocation(FVector(0, 0, 10));

	OnTakePointDamage.AddDynamic(this, &AJoyWayCharacter::OnDamage);
	CurrentHealth = MaxHealth;
}

void AJoyWayCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJoyWayCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AJoyWayCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AJoyWayCharacter::Reload);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJoyWayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJoyWayCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AJoyWayCharacter::OnSprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AJoyWayCharacter::OnSprintEnd);
}

void AJoyWayCharacter::StartFire()
{
	UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(GetComponentByClass(UWeaponComponent::StaticClass()));
	if (IsValid(WeaponComponent))
	{
		WeaponComponent->ServerFire(true);
	}
}

void AJoyWayCharacter::StopFire()
{
	UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(GetComponentByClass(UWeaponComponent::StaticClass()));
	if (IsValid(WeaponComponent))
	{
		WeaponComponent->ServerFire(false);
	}
}

void AJoyWayCharacter::Reload()
{
	UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(GetComponentByClass(UWeaponComponent::StaticClass()));
	if (IsValid(WeaponComponent))
	{
		WeaponComponent->ServerReload();
	}
}

void AJoyWayCharacter::OnDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
                                AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0)
	{
		MulticastKill();
		OnKilled.Broadcast(this);
	}
	// UE_LOG(LogTemp, Log, TEXT("Char: %s, Damage: %.2f, Health: %.2f"), *GetName(), Damage, CurrentHealth);
}

void AJoyWayCharacter::MulticastKill_Implementation()
{
	bDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	DisableInput(Cast<APlayerController>(GetController()));
}

void AJoyWayCharacter::OnSprintStart()
{
	ServerSetWalkMode(EWalkMode::Sprint);
}

void AJoyWayCharacter::OnSprintEnd()
{
	ServerSetWalkMode(EWalkMode::Walk);
}

void AJoyWayCharacter::ServerSetWalkMode_Implementation(EWalkMode NewWalkMode)
{
	WalkMode = NewWalkMode;
	OnRep_WalkMode();
}

void AJoyWayCharacter::OnRep_WalkMode()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeeds[WalkMode];
}

void AJoyWayCharacter::MoveForward(float Value)
{
	const FVector ForwardVector = FRotator(0, GetControlRotation().Yaw, 0).Vector();
	AddMovementInput(ForwardVector, Value);
}

void AJoyWayCharacter::MoveRight(float Value)
{
	const FVector RightVector = FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void AJoyWayCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJoyWayCharacter, WalkMode);
}