// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JoyWayCharacter.h"
#include "GameFramework/GameMode.h"
#include "JoyWayGameMode.generated.h"

UCLASS(minimalapi)
class AJoyWayGameMode : public AGameMode
{
	GENERATED_BODY()

	FTimerHandle DestroyCharacterTimer;

	UPROPERTY(EditDefaultsOnly, Category=JoyWay)
	float TimeToRespawn = 5.f;
public:
	AJoyWayGameMode();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
protected:

	UFUNCTION()
	void OnCharacterKilled(AJoyWayCharacter* KilledCharacter);
};



