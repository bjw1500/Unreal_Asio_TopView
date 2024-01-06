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

void ClientPacketHandler::OnRecvPacket(ServerSessionRef session, BYTE* buffer, int32 len)
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

void ClientPacketHandler::HandlePacket(PacketMessage packet)
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
		case Protocol::S_CREATEACCOUNT:
			Handle_S_CreateAccount(packet);
			break;
		case Protocol::S_FAILEDLOGIN:
			Handle_S_FailedLogin(packet);
			break;
		case Protocol::S_CHAT:
			Handle_S_Chat(packet);
	default:
		break;
	}
}



void ClientPacketHandler::Handle_S_TEST(PacketMessage packet)
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

void ClientPacketHandler::Handle_S_Disconnect(PacketMessage packet)
{

	Protocol::S_Disconnect pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
	FString text = FString::Printf(TEXT("ID[% d], Name[% s]의 연결이 끊어졌습니다.\n"), pkt.info().id(), pkt.info().name().c_str());
	Utils::DebugLog(text);


}

void ClientPacketHandler::Handle_S_Chat(PacketMessage packet)
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

void ClientPacketHandler::Handle_S_Connect(PacketMessage packet)
{
	Protocol::S_Connect pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

	//서버에서 보내준 String  파싱
	//FString text = FString::Printf(TEXT("%s"), pkt.info().c_str());
	FString text = UTF8_TO_TCHAR(pkt.info().c_str());
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, *text);

	//연결이 되었다. 이 시점에서 현재 사용자의 위치는 Title 화면.
	//Title 화면에 어떻게 정보를 전달해야할까?
	//일단 BP_Title을 가져와야 한다.
	//UWorld* world = GameInstance->GetWorld();
	//AMMORPG2GameModeBase* mode = Cast<AMMORPG2GameModeBase>(UGameplayStatics::GetGameMode(world));
	//if (IsValid(mode) == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Handle_S_Connect ERROR! Don't find mode!"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Handle_S_Connect ERROR! Don't find mode!"));
	//	return;
	//}

	//UTitleWidget* title = Cast<UTitleWidget>(mode->Main_UI);
	//if (IsValid(title) == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Handle_S_Connect ERROR! Don't find Title Widget"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Handle_S_Connect ERROR! Don't find Title Widget"));
	//	return;
	//}

	//FText serverText = FText::FromString(text);
	//title->Information_Text->SetText(serverText);
	//title->CurrentState = TitleState::Login;
	//title->IP_EditableText->SetVisibility(ESlateVisibility::Hidden);
	//title->ID_EditableText->SetVisibility(ESlateVisibility::Visible);
	//title->Password_EditableText->SetVisibility(ESlateVisibility::Visible);
	//title->CreateAccount_Button->SetVisibility(ESlateVisibility::Visible);
	//타이틀 화면에 서버 연결이 되었다는 정보를 전달한 후, 

	//로그인 절차에 들어간다.

	//이후 로그인 절차가 끝나면 레벨 이동 후 캐릭터 생성
}

void ClientPacketHandler::Handle_S_SuccessLogin(PacketMessage packet)
{
	//Protocol::S_SuccessLogin pkt;
	//PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	//pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));


	////서버에서 날아온 string 변환
	//FString text = UTF8_TO_TCHAR(pkt.info().c_str());

	////현재 화면의 UI 가져오기
	//UWorld* world = GameInstance->GetWorld();
	//AMMORPG2GameModeBase* mode = Cast<AMMORPG2GameModeBase>(UGameplayStatics::GetGameMode(world));
	//if (IsValid(mode) == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Handle_S_Connect ERROR! Don't find mode!"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Handle_S_Connect ERROR! Don't find mode!"));
	//	return;
	//}

	//UTitleWidget* title = Cast<UTitleWidget>(mode->Main_UI);
	//if (IsValid(title) == false)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Handle_S_Connect ERROR! Don't find Title Widget"));
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Handle_S_Connect ERROR! Don't find Title Widget"));
	//	return;
	//}

	//FText serverText = FText::FromString(text);
	//title->Information_Text->SetText(serverText);

	////이 단계에 들어갔다는 건 서버에서 아이디와 비밀번호를 받고, 로그인에 성공했다는 것.
	////Title 상태를 바꿔서, Login 확인 패킷을 여러번 보낼 수 없게 만들어준다.
	//title->CurrentState = TitleState::LoginSuccess;

	////TODO
	////나중에 Load 화면을 넣어주자. 
	//GameInstance->LoadGameLevel(FString(TEXT("Game")));

	//FTimerHandle WaitHandle;
	//float WaitTime = 1.0f; //시간을 설정하고
	//GameInstance->GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	//	{


	//		//화면 이동이 끝났으면, 서버에 캐릭터 생성 요청을 날린다.
	//		Make_C_EnterRoom();

	//	}), WaitTime, false); 
		
}

void ClientPacketHandler::Handle_S_CreateAccount(PacketMessage packet)
{
	Protocol::S_CreateAccount pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
}

void ClientPacketHandler::Handle_S_FailedLogin(PacketMessage packet)
{
	Protocol::S_FailedLogin pkt;
	PacketHeader* header = (PacketHeader*)packet.pkt.GetData();
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
}


SendBufferRef ClientPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack)
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

void ClientPacketHandler::Make_C_TryLogin(FString id, FString password)
{
	Protocol::C_TryLogin pkt;
	pkt.set_id(TCHAR_TO_UTF8(*id));
	pkt.set_password(TCHAR_TO_UTF8(*password));
	SendBufferRef sendBuffer = MakeSendBuffer(pkt, Protocol::C_TRYLOGIN);
	GameInstance->GetNetworkManager()->SendPacket(sendBuffer);
}

void ClientPacketHandler::Make_C_EnterRoom(int32 characterId)
{
	Protocol::C_EnterRoom pkt;
	pkt.set_characterid(characterId);
	SendBufferRef sendbuffer = MakeSendBuffer(pkt, Protocol::C_ENTER_ROOM);

	GameInstance->GetNetworkManager()->SendPacket(sendbuffer);

}

void ClientPacketHandler::Make_C_CreateAccount(FString id, FString password)
{
	Protocol::C_CreateAccount pkt;
	pkt.set_id(TCHAR_TO_UTF8(*id));
	pkt.set_password(TCHAR_TO_UTF8(*password));
	SendBufferRef sendBuffer = MakeSendBuffer(pkt, Protocol::C_CREATEACCOUNT);
	GameInstance->GetNetworkManager()->SendPacket(sendBuffer);
}
