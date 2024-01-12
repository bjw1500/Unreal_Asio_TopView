// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG2_API UUtilBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	
	//Packet 날리기
	UFUNCTION(BlueprintCallable, Category = "Server")
	static void SendEnterFieldPacket(int CharacterID = 0);
};
