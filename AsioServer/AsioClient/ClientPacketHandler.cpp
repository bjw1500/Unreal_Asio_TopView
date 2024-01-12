#include "pch.h"
#include "ClientPacketHandler.h"
#include "AsioClient.h"



PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(SessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_Connect(SessionRef& session, Protocol::S_Connect& pkt)
{
	std::cout << "Handle Connect" << endl;

	//Protocol::S_CHAT chatPkt;
	//chatPkt.set_msg(pkt.msg());
	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	return true;
}

SendBufferRef PacketHandler::Make_C_Test()
{
	Protocol::C_TryLogin pkt;
	pkt.set_id("test");
	return MakeSendBuffer(pkt, Protocol::C_TRYLOGIN);
}
