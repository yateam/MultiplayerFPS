// Copyright Epic Games, Inc. All Rights Reserved.

#include "JoyWayGameMode.h"
#include "JoyWayCharacter.h"
#include "JoyWayPlayerController.h"

AJoyWayGameMode::AJoyWayGameMode()
	: Super()
{
	// use our custom HUD class
	PlayerControllerClass = AJoyWayPlayerController::StaticClass();
	DefaultPawnClass = AJoyWayCharacter::StaticClass();
	bUseSeamlessTravel = true;
}

void AJoyWayGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

APawn* AJoyWayGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* Pawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
	AJoyWayCharacter* Character = Cast<AJoyWayCharacter>(Pawn);
	Character->OnKilled.AddUObject(this, &AJoyWayGameMode::OnCharacterKilled);
	
	return Pawn;
}

void AJoyWayGameMode::OnCharacterKilled(AJoyWayCharacter* KilledCharacter)
{
	GetWorldTimerManager().SetTimer(DestroyCharacterTimer, [this, KilledCharacter]
		{
			AController* Controller = KilledCharacter->GetController();
			KilledCharacter->Destroy();
			RestartPlayer(Controller);
		}, TimeToRespawn, false);
}

