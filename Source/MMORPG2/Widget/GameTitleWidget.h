// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseWidget.h"
#include "GameTitleWidget.generated.h"

/**
 * 
 */

 //타이틀에서는 서버 접속에 필요한 IP 주소와 플레이어 아이디를 받아줄 것이다.
UENUM(BlueprintType)
enum class TitleState : uint8
{
	Connect,
	Login,
	CreateAccount,
	LoginSuccess
};

UCLASS()
class MMORPG2_API UGameTitleWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
		UFUNCTION(BlueprintCallable, Category = "Server")
		bool TryConnectToServer(UPARAM(ref) FString& ServerAddress);

		UFUNCTION(BlueprintCallable, Category = "Server")
		bool TryConnectToLogin(UPARAM(ref) FString& ID, UPARAM(ref) FString& Password);

		UFUNCTION(BlueprintCallable, Category = "Server")
		bool TryCreateAccount(UPARAM(ref) FString& ID, UPARAM(ref) FString& Password);

		//Event

		UFUNCTION(BlueprintImplementableEvent, Category = "Server")
		bool OnSuccessConnect(UPARAM(ref) FString& message);

		UFUNCTION(BlueprintImplementableEvent, Category = "Server")
		bool OnSuccessLogin(UPARAM(ref) FString& message);

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TitleState CurrentState = TitleState::Connect;
	
};
