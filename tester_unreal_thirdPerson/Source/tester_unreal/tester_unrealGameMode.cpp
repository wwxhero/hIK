// Copyright Epic Games, Inc. All Rights Reserved.

#include "tester_unrealGameMode.h"
#include "tester_unrealCharacter.h"
#include "UObject/ConstructorHelpers.h"

Atester_unrealGameMode::Atester_unrealGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
