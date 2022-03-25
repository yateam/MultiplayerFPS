// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOYWAY_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
	AWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	FWeaponData StartupWeapon;

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ArmWeapon(FWeaponData WeaponData);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerFire(bool bStart);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void GetAmmoInfo(int32& CurrentAmmo, int32& MaxAmmo) const;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
