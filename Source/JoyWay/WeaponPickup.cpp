// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "JoyWayCharacter.h"
#include "WeaponComponent.h"

// Sets default values
AWeaponPickup::AWeaponPickup()
{
}

void AWeaponPickup::BeginOverlapByCharacter(AJoyWayCharacter* Character)
{
	UWeaponComponent* WeaponComponent = Cast<UWeaponComponent>(Character->GetComponentByClass(UWeaponComponent::StaticClass()));
	if (IsValid(WeaponComponent))
	{
		WeaponComponent->ArmWeapon(WeaponData);
	}
}


