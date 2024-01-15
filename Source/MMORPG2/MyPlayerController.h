// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Network/Protocol.pb.h"
#include "Network/FWorker.h"
#include "MyPlayerController.generated.h"


class UInputMappingContext;

UCLASS()
class MMORPG2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
   AMyPlayerController();

protected:
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaSeconds) override;
   virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


 private:
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerInputContext;

public:
	/*네트워크 관련 모음*/

	void SendPlayerInfo();

public:

	bool bCanControll = false;
};
