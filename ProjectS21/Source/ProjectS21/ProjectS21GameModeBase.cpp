// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectS21GameModeBase.h"
#include "Character/PlayerCharacter.h"

AProjectS21GameModeBase::AProjectS21GameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

