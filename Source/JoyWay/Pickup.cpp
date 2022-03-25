// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

APickup::APickup()
{
	bReplicates = true;
}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AJoyWayCharacter* Character = Cast<AJoyWayCharacter>(OtherActor);
	if (!IsValid(Character))
	{
		return;
	}

	if (HasAuthority() && bActive)
	{
		bActive = false;
		MulticastActivate(false);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &APickup::OnSpawn, SpawnPeriod);
		BeginOverlapByCharacter(Character);
	}
}

void APickup::OnSpawn()
{
	bActive = true;
	MulticastActivate(true);
}

void APickup::MulticastActivate_Implementation(bool bActivate)
{
	OnActivate(bActivate);
}



