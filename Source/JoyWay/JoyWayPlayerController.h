// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JoyWayCharacter.h"
#include "GameFramework/PlayerController.h"
#include "JoyWayPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=JoyWay)
	float Health = 0.f;

	UPROPERTY(BlueprintReadOnly, Category=JoyWay)
	int32 CurrentAmmo = 0;

	UPROPERTY(BlueprintReadOnly, Category=JoyWay)
	int32 MaxAmmo = 0;
};
/**
 * 
 */
UCLASS()
class JOYWAY_API AJoyWayPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* PossessedPawn) override;

	UFUNCTION(Client, Reliable)
	void ClientShowHUD();

	UFUNCTION(Client, Reliable)
	void ClientHideHUD();

	UFUNCTION(Client, Unreliable)
	void ClientUpdatePlayerInfo(FPlayerInfo PlayerInfo);

	UFUNCTION()
	void OnCharacterKilled(AJoyWayCharacter* KilledCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category=JoyWay)
	void OnShowHUD();

	UFUNCTION(BlueprintImplementableEvent, Category=JoyWay)
	void OnHideHUD();

	UFUNCTION(BlueprintImplementableEvent, Category=JoyWay)
	void OnUpdatePlayerInfo(FPlayerInfo PlayerInfo);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category=JoyWay)
	void ServerTravel(const FString& MapName);

public:
	virtual void Tick(float DeltaSeconds) override;
};
