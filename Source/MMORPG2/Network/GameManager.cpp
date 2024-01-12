// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "SocketUtils.h"
#include "Service.h"
#include "NetworkSession.h"
#include "Managers/NetworkManager.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Managers/UIManager.h"

UGameManager* GameInstance = nullptr;

UGameManager::~UGameManager()
{
	Utils::DebugLog(TEXT("~UGameManager"));
	ThreadManager::GetInstance()->Join();
	GetPacketQueue()->PopAll();
}

void UGameManager::Init()
{
	Super::Init();
	GameInstance = this;
	Utils::DebugLog(TEXT("UGameManager Init"));

}

void UGameManager::LoadGameLevel(FString name)
{
	Utils::DebugLog(name);
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), *name, true);

}


NetworkManager* UGameManager::GetNetworkManager()
{
	if (_netWorkManager.IsValid() == false)
	{
		_netWorkManager = MakeShared<NetworkManager>();
	}


	return _netWorkManager.Get();
}

UUIManager* UGameManager::GetUIManager()
{
	if (IsValid(UIManager)== false)
	{
		UIManager = NewObject<UUIManager>();
	}

	return UIManager;
}

ClientPacketHandler* UGameManager::GetPacketHandler()
{
	if (_netWorkManager.IsValid() == false)
	{
		_packetHandler = MakeShared<ClientPacketHandler>();
	}


	return _packetHandler.Get();
}

PacketQueue* UGameManager::GetPacketQueue()
{
	if (_packetQueue.IsValid() == false)
	{

		_packetQueue = MakeShared<PacketQueue>();

	}
	return _packetQueue.Get();
}

void Utils::DebugLog(FString string)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, *string);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *string);
	}
}
