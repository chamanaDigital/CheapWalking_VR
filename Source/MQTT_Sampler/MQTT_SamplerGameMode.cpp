// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MQTT_SamplerGameMode.h"
#include "MQTT_SamplerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMQTT_SamplerGameMode::AMQTT_SamplerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
