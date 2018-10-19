// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProject6GameMode.h"
#include "MyProject6Character.h"
#include "UObject/ConstructorHelpers.h"

AMyProject6GameMode::AMyProject6GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
