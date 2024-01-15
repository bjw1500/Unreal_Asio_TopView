// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPlayerController.h"
#include "Network/Protocol.pb.h"
#include "Network/NetworkSession.h"
#include "Network/Buffer.h"
#include "Network/FWorker.h"
#include "Network/Service.h"
#include "Network/BufferHelper.h"
#include "Network/SocketUtils.h"
#include "Managers/NetworkManager.h"
#include "Network/ClientPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Network/GameManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


AMyPlayerController::AMyPlayerController()
{

}

void AMyPlayerController::BeginPlay()
{
	//GameInstance->GetNetworkManager()->Init();
	Super::BeginPlay();

	check(PlayerInputContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//뒤에 숫자는 입력 우선 순위.
		Subsystem->AddMappingContext(PlayerInputContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(GetLocalPlayer())) {

		//Jumping
	//	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	}


}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	
	TArray<PacketMessage> list = GameInstance->GetPacketQueue()->PopAll();
	for (PacketMessage message : list)
	{
		GameInstance->GetPacketHandler()->HandlePacket(message);
	}

	if (GameInstance->bConnected == false)
		return;
	if (bCanControll == false)
		return;
	SendPlayerInfo();

}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//GameInstance->GetNetworkManager()->Disconnect();
	//GameInstance->GetPacketHandler()->Make_C_DisConnect(TEXT("Game End"));

}




///////////Network

void AMyPlayerController::SendPlayerInfo()
{
	//ACreature* player = Cast<ACreature>(UGameplayStatics::GetPlayerCharacter(this, 0));
	//if (IsValid(player) == false)
	//	return;

	//const auto& location = player->GetActorLocation();
	//const auto & rotation = player->GetActorRotation();
	//const auto & velocity = player->GetVelocity();

	//ObjectInfo를 어떻게 해줄까?
	//Protocol::ObjectInfo* newInfo = player->GetInfo();
	//Protocol::Position* updatePosition = newInfo->mutable_position();

	//updatePosition->set_locationx(location.X);
	//updatePosition->set_locationy(location.Y);
	//updatePosition->set_locationz(location.Z);

	//updatePosition->set_rotationx(rotation.Roll);
	//updatePosition->set_rotationy(rotation.Pitch);
	//updatePosition->set_rotationz(rotation.Yaw);

	//updatePosition->set_velocityx(velocity.X);
	//updatePosition->set_velocityy(velocity.Y);
	//updatePosition->set_velocityz(velocity.Z);

	//서버와 연결된 상태가 아니라면, 
	//if (GameInstance->bConnected == false)
	//	return;

	//GameInstance->GetPacketHandler()->Make_C_Move(*newInfo);
}


