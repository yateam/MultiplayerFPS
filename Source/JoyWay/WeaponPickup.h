// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Weapon.h"
#include "WeaponPickup.generated.h"

UCLASS()
class JOYWAY_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=JoyWay, meta=(AllowPrivateAccess))
	FWeaponData WeaponData;
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

	virtual void BeginOverlapByCharacter(AJoyWayCharacter* Character) override;
};
