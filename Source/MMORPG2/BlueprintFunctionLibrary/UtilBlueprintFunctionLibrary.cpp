// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilBlueprintFunctionLibrary.h"
#include "Network/GameManager.h"
#include "Network/ClientPacketHandler.h"

void UUtilBlueprintFunctionLibrary::SendEnterFieldPacket(int CharacterID)
{
	GameInstance->GetPacketHandler()->Make_C_EnterField(CharacterID);
}
