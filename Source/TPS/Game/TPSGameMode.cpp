// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameMode.h"
#include "TPSPlayerController.h"
#include "../Character/TPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPSGameMode::ATPSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATPSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TPS/Blueprint/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATPSGameMode::PlayerCharacterDead()
{

}