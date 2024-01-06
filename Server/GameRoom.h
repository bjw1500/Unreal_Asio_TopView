#pragma once
#include "Protocol.pb.h"

class GameRoom : public enable_shared_from_this<GameRoom>
{
public:
	GameRoom();
	~GameRoom();

	void Update(float deltaTime);

public:
	PlayerRef CreatePlayer(GameSessionRef session);
	GameRoomRef GetRoomRef() {
		return shared_from_this();
	}


	};

extern 	atomic<int32> _objectId;
extern GameRoomRef GRoom;

