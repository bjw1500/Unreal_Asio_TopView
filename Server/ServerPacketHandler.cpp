#include "pch.h"
#include <format>
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "Player.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "DBConnection.h"
#include "DBConnectionPool.h"
#include "Account.h"


void ServerPacketHandler::HandlePacket(GameSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//	uint16 id = header->id;
	uint16 size = header->size;

	switch (header->id)
	{
	//case Protocol::C_ENTER_ROOM:
	//	Handle_C_EnterRoom(session, buffer, len);
		break;
	default:
		break;
	}

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

