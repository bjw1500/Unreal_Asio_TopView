// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPacketHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "Managers/NetworkManager.h"
#include "../MyPlayerController.h"
#include "BufferHelper.h"
#include "Buffer.h"
#include "Protocol.pb.h"
#include "../MMORPG2GameModeBase.h"
#include "../TitleWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"
#include "Widget/GameTitleWidget.h"

void UClientPacketHandler::OnRecvPacket(ServerSessionRef session, BYTE* buffer, int32 len)
{
   PacketHeader* header = (PacketHeader*)buffer;
//	uint16 id = header->id;
	uint16 size = header->size;

	PacketMessage message;
	message.pkt.AddUninitialized(len);
	FMemory::Memcpy(message.pkt.GetData(),buffer, len);
	GameInstance->GetPacketQueue()->Push(message);

	//FString string = FString::Printf(TEXT("Size[%d] Len[%d] TArray.num[%d]"), size, len, message.pkt.GetAllocatedSize());
	//Utils::DebugLog(string);
}

void UClientPacketHandler::HandlePacket(PacketMessage packet)
{
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	uint16 id = header->id;

	//FString string = FString::Printf(TEXT("Packet Id[%d]"), id);
	//Utils::DebugLog(string);

	switch (id)
	{
		case Protocol::S_TEST:
			Handle_S_TEST(packet);
			break;
		case Protocol::S_CONNECT:
			Handle_S_Connect(packet);
			break;
		case Protocol::S_SUCCESSLOGIN:
			Handle_S_SuccessLogin(packet);
			break;
		case Protocol::S_FAILEDLOGIN:
			Handle_S_FailedLogin(packet);
			break;
		case Protocol::S_CHAT:
			Handle_S_Chat(packet);
			break;
		case Protocol::S_ENTER_FIELD:
			Handle_S_EnterField(packet);
			break;
	default:
		break;
	}
}



void UClientPacketHandler::Handle_S_TEST(PacketMessage packet)
{
	Protocol::S_Test pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
	int32 id = pkt.id();
	int32 hp = pkt.hp();
	int32 attack = pkt.attack();

	FString text = FString::Printf(TEXT("%d %d %d"), id, hp, attack);
	Utils::DebugLog(text);
}

void UClientPacketHandler::Handle_S_Disconnect(PacketMessage packet)
{

	Protocol::S_Disconnect pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
	//FString text = FString::Printf(TEXT("ID[% d], Name[% s]의 연결이 끊어졌습니다.\n"), pkt.info().id(), pkt.info().name().c_str());
	//Utils::DebugLog(text);


}

void UClientPacketHandler::Handle_S_Chat(PacketMessage packet)
{
	Protocol::S_Chat pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));


	////채팅이 날아왔다.
	//FString text = UTF8_TO_TCHAR(pkt.msg().c_str());
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, *text);
	//UWorld* world = GameInstance->GetWorld();
	//AMMORPG2GameModeBase* mode = Cast<AMMORPG2GameModeBase>(UGameplayStatics::GetGameMode(world));
	//if (IsValid(mode) == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Handle_S_Connect ERROR! Don't find mode!"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Handle_S_Connect ERROR! Don't find mode!"));
	//	return;
	//}

	//UChatWidget* chat = Cast<UChatWidget>(mode->Main_UI);
	//if (IsValid(chat) == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Handle_S_Chat ERROR! Don't find chat Widget"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Handle_S_Chat ERROR! Don't find chat Widget"));
	//	return;
	//}
	//chat->UpdateChat(text);


}

void UClientPacketHandler::Handle_S_EnterField(PacketMessage packet)
{
	Protocol::S_EnterField pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

	if(GameInstance->bConnected == false)
		return;

	Utils::DebugLog(TEXT("Handle S EnterField"));

}

void UClientPacketHandler::Handle_S_Connect(PacketMessage packet)
{
	Protocol::S_Connect pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

	//서버에서 보내준 String  파싱
	//FString text = FString::Printf(TEXT("%s"), pkt.info().c_str());
	FString connectText = UTF8_TO_TCHAR(pkt.info().c_str());

	/*
		플레이어의 스테이지 위치가 Title인지 나중에 확인하는 절차 넣기.
		Title UI를 가져와야 하는데 어떻게 가져올까?
	*/

	UGameTitleWidget* gameTitleUI = Cast<UGameTitleWidget>(GameInstance->GetUIManager()->Main_UI);
	if (IsValid(gameTitleUI) == false)
	{
		Utils::DebugLog(TEXT("Error Handle_S_Connect"));
		return;
	}

	gameTitleUI->CurrentState = TitleState::Login;
	gameTitleUI->OnSuccessConnect(connectText);

	

}

void UClientPacketHandler::Handle_S_SuccessLogin(PacketMessage packet)
{
	Protocol::S_SuccessLogin pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));


	////서버에서 날아온 string 변환
	FString text = UTF8_TO_TCHAR(pkt.info().c_str());
	UGameTitleWidget* gameTitleUI = Cast<UGameTitleWidget>(GameInstance->GetUIManager()->Main_UI);
	gameTitleUI->CurrentState = TitleState::LoginSuccess;
	gameTitleUI->OnSuccessLogin(text);
}

void UClientPacketHandler::Handle_S_FailedLogin(PacketMessage packet)
{
	Protocol::S_FailedLogin pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
}


SendBufferRef UClientPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack)
{
	Protocol::S_Test pkt;

	pkt.set_id(10);
	pkt.set_hp(100);
	pkt.set_attack(10);


	return MakeSendBuffer(pkt, Protocol::S_TEST);
}


//void ClientPacketHandler::Make_C_Chat(FString msg)
//{
//	Protocol::C_Chat pkt;
//	int32 playerId = GameInstance->GetObjectManager()->GetMyPlayer()->GetInfo()->id();
//	pkt.set_id(playerId);
//	pkt.set_msg(TCHAR_TO_UTF8(*msg));
//}

void UClientPacketHandler::Make_C_TryLogin(FString id, FString password)
{
	Protocol::C_TryLogin pkt;
	pkt.set_id(TCHAR_TO_UTF8(*id));
	pkt.set_password(TCHAR_TO_UTF8(*password));
	SendBufferRef sendBuffer = MakeSendBuffer(pkt, Protocol::C_TRYLOGIN);
	GameInstance->GetNetworkManager()->SendPacket(sendBuffer);
}

void UClientPacketHandler::Make_C_EnterField(int32 characterId)
{
	Protocol::C_EnterField pkt;
	pkt.set_characterid(characterId);
	SendBufferRef sendbuffer = MakeSendBuffer(pkt, Protocol::C_ENTER_FIELD);

	GameInstance->GetNetworkManager()->SendPacket(sendbuffer);
}

void UClientPacketHandler::Make_C_DisConnect(FString reason)
{
	Protocol::C_Disconnect pkt;
	SendBufferRef sendBuffer = MakeSendBuffer(pkt, Protocol::C_DISCONNECT);
	GameInstance->GetNetworkManager()->SendPacket(sendBuffer);

}
