// Fill out your copyright notice in the Description page of Project Settings.


#include "JoyWayPlayerController.h"

#include "JoyWayCharacter.h"
#include "WeaponComponent.h"

void AJoyWayPlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	ClientShowHUD();
	AJoyWayCharacter* PlayerCharacter = Cast<AJoyWayCharacter>(PossessedPawn);
	PlayerCharacter->OnKilled.AddUObject(this, &AJoyWayPlayerController::OnCharacterKilled);
}

void AJoyWayPlayerController::OnCharacterKilled(AJoyWayCharacter* KilledCharacter)
{
	ClientHideHUD();
}

void AJoyWayPlayerController::ServerTravel_Implementation(const FString& MapName)
{
	GetWorld()->ServerTravel(MapName);
}

void AJoyWayPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		const AJoyWayCharacter* PlayerCharacter = Cast<AJoyWayCharacter>(GetPawn());
		if (IsValid(PlayerCharacter))
		{
			FPlayerInfo PlayerInfo;
			PlayerInfo.Health = PlayerCharacter->GetCurrentHealth();

			const UWeaponComponent* WeaponComponent =
				Cast<UWeaponComponent>(PlayerCharacter->GetComponentByClass(UWeaponComponent::StaticClass()));
			if (IsValid(WeaponComponent))
			{
				int32 CurrentAmmo, MaxAmmo;
				WeaponComponent->GetAmmoInfo(CurrentAmmo, MaxAmmo);
				PlayerInfo.CurrentAmmo = CurrentAmmo;
				PlayerInfo.MaxAmmo = MaxAmmo;
			}

			ClientUpdatePlayerInfo(PlayerInfo);
		}
	}
}

void AJoyWayPlayerController::ClientUpdatePlayerInfo_Implementation(FPlayerInfo PlayerInfo)
{
	OnUpdatePlayerInfo(PlayerInfo);
}

void AJoyWayPlayerController::ClientHideHUD_Implementation()
{
	OnHideHUD();
}

void AJoyWayPlayerController::ClientShowHUD_Implementation()
{
	
	OnShowHUD();
}
