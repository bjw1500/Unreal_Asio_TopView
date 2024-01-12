#pragma once
#include "Session.h"

using PacketHandlerFunc = std::function<bool(ClientSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


bool Handle_INVALID(ClientSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_CHAT(ClientSessionRef& session, Protocol::C_Chat& pkt);
bool Handle_C_TryLogin(ClientSessionRef& session, Protocol::C_TryLogin& pkt);
bool Handle_C_EnterField(ClientSessionRef session, Protocol::C_EnterField& pkt);
bool Handle_C_Disconnect(ClientSessionRef session, Protocol::C_Disconnect& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[Protocol::C_CHAT] = [](ClientSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_Chat>(Handle_C_CHAT, session, buffer, len); };
		GPacketHandler[Protocol::C_TRYLOGIN] = [](ClientSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_TryLogin>(Handle_C_TryLogin, session, buffer, len); };
		GPacketHandler[Protocol::C_ENTER_FIELD] = [](ClientSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_EnterField>(Handle_C_EnterField, session, buffer, len); };
		GPacketHandler[Protocol::C_DISCONNECT] = [](ClientSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_Disconnect>(Handle_C_Disconnect, session, buffer, len); };
	}
	static SendBufferRef Make_S_Connect();

	static bool HandlePacket(ClientSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

public:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, ClientSessionRef& session, BYTE* buffer, int32 len)
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

		//cout << pktId << " " << "packetSize [" << packetSize << "]" << endl;

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		assert(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};