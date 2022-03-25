// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=JoyWay)
	TSubclassOf<class AWeapon> WeaponClass;
};

UCLASS(Abstract)
class JOYWAY_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=JoyWay, meta=(AllowPrivateAccess))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=JoyWay, meta=(AllowPrivateAccess))
	UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	float MaxDistance = 3000.f;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	int32 MaxAmmo;

	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	float Rate = 2.f;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	float Damage = 20.f;

	float LastTimeFired = 0.f;

	bool bFiring = false;

protected:
	void DoFire();

	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void Tick(float DeltaSeconds) override;

	void Fire(bool bStart);

	void Reload();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEffects();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayHitEffects(FHitResult HitResult);

	UFUNCTION(BlueprintImplementableEvent, Category=JoyWay)
	void PlayEffects();

	UFUNCTION(BlueprintImplementableEvent, Category=JoyWay)
	void PlayHitEffects(FHitResult HitResult);

	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
};
