// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "JoyWayCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FKillDelegate, class AJoyWayCharacter*);

UENUM()
enum class EWalkMode : uint8
{
	Walk,
	Sprint
};

UCLASS(config=Game)
class AJoyWayCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(ReplicatedUsing=OnRep_WalkMode)
	EWalkMode WalkMode = EWalkMode::Walk;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category=JoyWay, meta=(AllowPrivateAccess))
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	TMap<EWalkMode, float> WalkSpeeds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=JoyWay, meta=(AllowPrivateAccess))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=JoyWay, meta=(AllowPrivateAccess))
	UCameraComponent* Camera;

	bool bDead;

public:
	FKillDelegate OnKilled;
	
	AJoyWayCharacter();

	UFUNCTION(BlueprintCallable, Category=JoyWay)
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category=JoyWay)
	FORCEINLINE bool IsDead() const { return bDead; }
	
protected:
	void StartFire();
	void StopFire();
	void Reload();

	void OnSprintStart();
	void OnSprintEnd();

	UFUNCTION(Server, Reliable)
	void ServerSetWalkMode(EWalkMode NewWalkMode);

	UFUNCTION()
	void OnRep_WalkMode();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
		class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
		const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastKill();
};

