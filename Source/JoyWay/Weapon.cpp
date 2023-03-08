// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "DrawDebugHelpers.h"
#include "JoyWayCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlash"));
	MuzzleFlash->SetupAttachment(WeaponMesh, TEXT("Muzzle"));
	MuzzleFlash->SetAutoActivate(false);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		if (bFiring && LastTimeFired + 1 / Rate <= GetWorld()->GetTimeSeconds())
		{
			LastTimeFired = GetWorld()->GetTimeSeconds();
			DoFire();
		}
	}
}

void AWeapon::DoFire()
{
	if (CurrentAmmo <= 0)
	{
		return;
	}
	
	// hit trace
	AJoyWayCharacter* Character = Cast<AJoyWayCharacter>(GetOwner());

	const FVector ForwardVector = Character->GetCamera()->GetForwardVector();
	const FVector Start = Character->GetCamera()->GetComponentLocation();
	const FVector End = Start + ForwardVector * MaxDistance;
	
	const TArray<AActor*> IgnoredActors = { Character };
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1) };
	
	FHitResult OutHit;
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(this, Start, End,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, IgnoredActors,
		EDrawDebugTrace::None, OutHit, true);
	if (bHit)
	{
		MulticastPlayHitEffects(OutHit);

		AJoyWayCharacter* OtherChar = Cast<AJoyWayCharacter>(OutHit.GetActor());
		if (IsValid(OtherChar))
		{
			const FVector HitDirection = (OutHit.Location - GetActorLocation()).GetSafeNormal();
			UGameplayStatics::ApplyPointDamage(OtherChar, Damage, HitDirection, OutHit,
				Character->GetController(), this, UDamageType::StaticClass());
		}
	}
	
	// client side effects
	MulticastPlayEffects();

	CurrentAmmo--;
}

void AWeapon::Fire(bool bStart)
{
	bFiring = bStart;
}

void AWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void AWeapon::MulticastPlayHitEffects_Implementation(FHitResult HitResult)
{
	PlayHitEffects(HitResult);
}

void AWeapon::MulticastPlayEffects_Implementation()
{
	MuzzleFlash->Deactivate();
	MuzzleFlash->Activate();
	PlayEffects();
}

