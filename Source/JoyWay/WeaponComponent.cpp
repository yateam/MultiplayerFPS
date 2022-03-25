// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "JoyWayCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority &&  *StartupWeapon.WeaponClass != nullptr)
	{
		ArmWeapon(StartupWeapon);
	}
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::ArmWeapon(FWeaponData WeaponData)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		CurrentWeapon->Destroy();
	}
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponData.WeaponClass);

	AJoyWayCharacter* Owner = Cast<AJoyWayCharacter>(GetOwner());
	if (!IsValid(Owner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to attach a weapon actor to non-JoyWayCharacter actor"));
		return;
	}

	CurrentWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Gun"));
	CurrentWeapon->SetOwner(Owner);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, CurrentWeapon);
}

void UWeaponComponent::GetAmmoInfo(int32& CurrentAmmo, int32& MaxAmmo) const
{
	if (IsValid(CurrentWeapon))
	{
		CurrentAmmo = CurrentWeapon->GetCurrentAmmo();
		MaxAmmo = CurrentWeapon->GetMaxAmmo();
	}
}

void UWeaponComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Destroy();
	}
}

void UWeaponComponent::ServerReload_Implementation()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Reload();
	}
}

void UWeaponComponent::ServerFire_Implementation(bool bStart)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Fire(bStart);
	}
}

