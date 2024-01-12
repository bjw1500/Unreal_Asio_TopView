#include "pch.h"
#include "ServerPacketHandler.h"
#include "DBConnection.h"
#include "DBConnectionPool.h"
#include "FieldManager.h"
#include "Field.h"
#include "Account.h"
#include "Player.h"
#include "AsioServer.h"



PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(ClientSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_CHAT(ClientSessionRef& session, Protocol::C_Chat& pkt)
{
	std::cout << pkt.msg().c_str() << endl;

	//Protocol::S_CHAT chatPkt;
	//chatPkt.set_msg(pkt.msg());
	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	return true;
}

bool Handle_C_TryLogin(ClientSessionRef& session, Protocol::C_TryLogin& pkt)
{

	//이미 로그인 된 상태라면 무시한다.
	if (session->GetAccountRef() != nullptr)
	{
		return false;
	}

	cout << "ID[" << pkt.id() << "]" << " 가 로그인을 시도합니다. 비밀번호" << pkt.password() << endl;

	/*
		계정 아이디가 맞는지 검증 절차를 나중에 추가해주자.
	*/

	DBConnection* db = GDBConnectionPool->Pop();
	string accountID = pkt.id();
	string accountPassword = pkt.password();
	int accountDB_Key = db->CheckAccount(accountID, accountPassword);

	//로그인 실패.
	if (accountDB_Key == false)
	{

		Protocol::S_FailedLogin failedPacket;
		failedPacket.set_info("잘못된 계정 정보 입니다. 다시 입력해주세요.");
		session->Send(ServerPacketHandler::MakeSendBuffer(failedPacket, Protocol::S_FAILEDLOGIN));
		return false;
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
	session->Send(ServerPacketHandler::MakeSendBuffer(pkt2, Protocol::S_SUCCESSLOGIN));
	return true;
}

bool Handle_C_EnterField(ClientSessionRef session, Protocol::C_EnterField& pkt)
{
	/*
	* TODO
		클라이언트에서 고른 캐릭터의 슬롯 정보를 보내올 것이다.
		슬롯 정보를 이용해서 현재 캐릭터 정보를 가져오고, 현재 위치한 필드에 입장시켜주자.
	*/

	//현재는 기본으로 생성해주는 테스트맵 ID 0으로 그냥 바로 진입.
	FieldRef currentField = GFieldManager->GetField(0);
	currentField->EnterPlayer(session);
	return true;
}

bool Handle_C_Disconnect(ClientSessionRef session, Protocol::C_Disconnect& pkt)
{
	session->OnDisconnected();
	return true;
}

SendBufferRef ServerPacketHandler::Make_S_Connect()
{
	Protocol::S_Connect pkt;
	pkt.set_info("서버 연결 성공");
	return MakeSendBuffer(pkt, Protocol::S_CONNECT);
}
