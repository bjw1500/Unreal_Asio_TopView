// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.h"
#include "NetworkSession.h"
#include "Managers/NetworkManager.h"

/**
 * 
 */

class MMORPG2_API ClientPacketHandler
{
public:
	void OnRecvPacket(ServerSessionRef session, BYTE* buffer, int32 len);
	void HandlePacket(PacketMessage packet);

	//IN Game
	void Handle_S_TEST(PacketMessage packet);
	void Handle_S_Disconnect(PacketMessage packet);
	void Handle_S_Chat(PacketMessage packet);
	void Handle_S_EnterField(PacketMessage packet);

	//Pre - Game
	void Handle_S_Connect(PacketMessage packet);
	void Handle_S_SuccessLogin(PacketMessage packet);
	//void Handle_S_CreateAccount(PacketMessage packet);
	void Handle_S_FailedLogin(PacketMessage packet);
	

	//In - Game
	SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack);
	
	//Pre - Game
	void Make_C_TryLogin(FString id, FString password);
	void Make_C_EnterField(int32 characterId = 0);
	void Make_C_DisConnect(FString reason);

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};

