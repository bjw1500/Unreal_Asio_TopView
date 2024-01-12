#pragma once
#include "Session.h"

using PacketHandlerFunc = std::function<bool(SessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(SessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_CHAT(SessionRef& session, Protocol::C_Chat& pkt);
bool Handle_C_TryLogin(SessionRef& session, Protocol::C_TryLogin& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[Protocol::C_CHAT] = [](SessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_Chat>(Handle_C_CHAT, session, buffer, len); };
		GPacketHandler[Protocol::C_TRYLOGIN] = [](SessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_TryLogin>(Handle_C_TryLogin, session, buffer, len); };
	}
	static SendBufferRef Make_S_Connect();

	static bool HandlePacket(SessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

public:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, SessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		cout << pktId << " " << "packetSize [" << packetSize << "]" << endl;

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		assert(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};