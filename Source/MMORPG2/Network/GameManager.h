// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Protocol.pb.h"
#include "MyPlayerController.h"
#include "GameManager.generated.h"

class NetworkManager;
class UClientPacketHandler;
class PacketQueue;
class UUIManager;
class UObjectManager;

UCLASS()
class MMORPG2_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	~UGameManager();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Server")
	void LoadGameLevel(FString name);



public:
	NetworkManager*								GetNetworkManager();

	UUIManager*											GetUIManager();
	UObjectManager*								GetObjectManager();

	UClientPacketHandler*						GetPacketHandler();
	PacketQueue*										GetPacketQueue();
	AMyPlayerController*							GetPlayerController() 
	{
		return Cast<AMyPlayerController>(GetFirstLocalPlayerController());
	}

private:
	TSharedPtr<NetworkManager>					_netWorkManager;
	TSharedPtr<PacketQueue>							_packetQueue;
	//TSharedPtr<ClientPacketHandler>			_packetHandler;

	//UPROPERTY()
	//TObjectPtr<UUIManager>								UIManager;

	UPROPERTY()
	AMyPlayerController* MyPlayerController = nullptr;

private:
	TSubclassOf<class ACreature> SpawnCharacter;

public:
	//서버 연결?
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Server")
	bool bOnline = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Server")
	bool bConnected = false;
};

//#define GameInstance  Cast<UGameManager>(GetGameInstance());

extern UGameManager* GameInstance;

class Utils
{
public:
	static void DebugLog(FString string);




};