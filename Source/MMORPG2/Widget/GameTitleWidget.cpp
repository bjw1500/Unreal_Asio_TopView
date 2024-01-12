// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTitleWidget.h"
#include "Network/GameManager.h"
#include "Managers/NetworkManager.h"
#include "Network/ClientPacketHandler.h"

bool UGameTitleWidget::TryConnectToServer(FString& ServerAddress)
{
	if (CurrentState != TitleState::Connect)
		return false;

	FString serverIP = ServerAddress;
	wstring ip = TCHAR_TO_WCHAR(*serverIP);
	bool connect = GameInstance->GetNetworkManager()->Init(ip);

	FString text = FString::Printf(TEXT("%s"), ip.c_str());

	Utils::DebugLog(text);

	return connect;
}

bool UGameTitleWidget::TryConnectToLogin(FString& ID, FString& Password)
{
	if (CurrentState != TitleState::Login)
		return false;

	FString id = ID;
	FString password = Password;

	GameInstance->GetPacketHandler()->Make_C_TryLogin(id, password);

	return true;
}

bool UGameTitleWidget::TryCreateAccount(FString& ID, FString& Password)
{
	if (CurrentState != TitleState::CreateAccount)
		return false;

	FString id = ID;
	FString password = Password;

	//GameInstance->GetPacketHandler()->Make_C_CreateAccount(id, password);
	return true;
}
