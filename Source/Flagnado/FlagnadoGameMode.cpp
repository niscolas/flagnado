// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlagnadoGameMode.h"
#include "FlagnadoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFlagnadoGameMode::AFlagnadoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
