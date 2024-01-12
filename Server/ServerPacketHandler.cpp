#include "pch.h"
#include <format>
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "DBConnection.h"
#include "DBConnectionPool.h"
#include "FieldManager.h"
#include "FieldSector.h"
#include "Field.h"
#include "Account.h"


void ServerPacketHandler::HandlePacket(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//	uint16 id = header->id;
	uint16 size = header->size;

	switch (header->id)
	{
	case Protocol::C_DISCONNECT:
		Handle_C_Disconnect(session, buffer, len);
		break;
		case Protocol::C_TRYLOGIN:
		Handle_C_TryLogin(session, buffer, len);
		break;
		case Protocol::C_ENTER_FIELD:
		Handle_C_EnterField(session, buffer, len);
		break;
	default:
		break;
	}

}

void ServerPacketHandler::Handle_C_Disconnect(GameSessionRef session, BYTE* buffer, int32 len)
{
	Protocol::C_Disconnect pkt;
	PacketHeader* header = (PacketHeader*)buffer;
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

	session->OnDisconnected();
}

void ServerPacketHandler::Handle_C_TryLogin(GameSessionRef session, BYTE* buffer, int32 len)
{
	Protocol::C_TryLogin pkt;
	PacketHeader* header = (PacketHeader*)buffer;
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));

	cout << "ID[" << pkt.id() << "]" << " 가 로그인을 시도합니다. 비밀번호" << pkt.password() << endl;

	/*
		계정 아이디가 맞는지 검증 절차를 나중에 추가해주자.
	*/

	DBConnection* db = GDBConnectionPool->Pop();
	string accountID =  pkt.id();
	string accountPassword = pkt.password();
	int accountDB_Key = db->CheckAccount(accountID, accountPassword);

	//로그인 실패.
	if (accountDB_Key == false)
	{

		Protocol::S_FailedLogin failedPacket;
		failedPacket.set_info("잘못된 계정 정보 입니다. 다시 입력해주세요.");
		session->Send(MakeSendBuffer(failedPacket, Protocol::S_FAILEDLOGIN));
		return;
	}


	//로그인에 성공했다면, Session에 계정 정보를 넣어주자.
	AccountRef sessionAccount = make_shared<Account>(accountID, accountDB_Key);
	session->SetAccount(sessionAccount);

	/*
	TODO
	계정 정보에 담긴 플레이어 데이터 로딩 과정 넣어주기.
	sessionAccount->LoadPlayerDB.
	*/

	Protocol::S_SuccessLogin pkt2;
	std::string str = std::format("{}님 환영합니다.", pkt.id());
	pkt2.set_info(str);
	session->Send(MakeSendBuffer(pkt2, Protocol::S_SUCCESSLOGIN));
}

void ServerPacketHandler::Handle_C_EnterField(GameSessionRef session, BYTE* buffer, int32 len)
{
	Protocol::C_EnterField pkt;
	PacketHeader* header = (PacketHeader*)buffer;
	pkt.ParseFromArray(&header[1], header->size - sizeof(PacketHeader));
	/*
	* TODO
		클라이언트에서 고른 캐릭터의 슬롯 정보를 보내올 것이다.
		슬롯 정보를 이용해서 현재 캐릭터 정보를 가져오고, 현재 위치한 필드에 입장시켜주자.
	*/

	//현재는 기본으로 생성해주는 테스트맵 ID 0으로 그냥 바로 진입.
	FieldRef currentField = GFieldManager->GetField(0);
	currentField->EnterPlayer(session);
}

SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack)
{
	Protocol::S_Test pkt;

	pkt.set_attack(attack);
	pkt.set_hp(hp);
	pkt.set_id(id);


	return MakeSendBuffer(pkt, Protocol::S_TEST);
}

SendBufferRef ServerPacketHandler::Make_S_Connect()
{
	Protocol::S_Connect pkt;
	pkt.set_info("서버 연결 성공");
	return MakeSendBuffer(pkt, Protocol::S_CONNECT);
}

