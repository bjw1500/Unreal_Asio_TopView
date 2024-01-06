#pragma once



using GameSessionRef = std::shared_ptr<class GameSession>;
class ServerPacketHandler
{
public:
	static void HandlePacket(GameSessionRef session, BYTE* buffer, int32 len);

	// ������
	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack);
	
	//pre - game
	static SendBufferRef Make_S_Connect();


	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		assert(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};