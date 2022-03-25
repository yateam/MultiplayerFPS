// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JoyWayCharacter.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS(Abstract)
class JOYWAY_API APickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=JoyWay)
	float SpawnPeriod = 5.f;

	FTimerHandle SpawnTimer;

	bool bActive = true;
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual void BeginOverlapByCharacter(AJoyWayCharacter* Character) {}

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivate(bool bActivate);

	UFUNCTION(BlueprintImplementableEvent, Category=JoyWay)
	void OnActivate(bool bActivate);

	void OnSpawn();	
};
