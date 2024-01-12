#include "pch.h"
#include "ServerPacketHandler.h"
#include "AsioServer.h"



PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(SessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_CHAT(SessionRef& session, Protocol::C_Chat& pkt)
{
	std::cout << pkt.msg().c_str() << endl;

	//Protocol::S_CHAT chatPkt;
	//chatPkt.set_msg(pkt.msg());
	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	return true;
}

bool Handle_C_TryLogin(SessionRef& session, Protocol::C_TryLogin& pkt)
{
	std::cout << pkt.id() << endl;
	return true;
}

SendBufferRef ServerPacketHandler::Make_S_Connect()
{
	Protocol::S_Connect pkt;
	return MakeSendBuffer(pkt, Protocol::S_CONNECT);
}
