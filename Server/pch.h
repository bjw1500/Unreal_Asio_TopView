#pragma once

#include "CorePch.h"
#include "Protocol.pb.h"


using GameSessionRef = std::shared_ptr<class GameSession>;
using GameRoomRef = std::shared_ptr<class GameRoom>;
using GameObjectRef = std::shared_ptr<class GameObject>;
using PlayerRef = std::shared_ptr<class Player>;
using MonsterRef = std::shared_ptr<class Monster>;
using CreatureRef = std::shared_ptr<class Creature>;
using AccountRef = std::shared_ptr<class Account>;
using FieldRef = shared_ptr<class Field>;
using FieldSectorRef = shared_ptr<class FieldSector>;
using ObjectInfoRef = shared_ptr<Protocol::ObjectInfo>;

using CharacterKey = int32;
using ObectID= int32;

using namespace Protocol;

extern class DBConnectionPool* GDBConnectionPool;
extern class FieldManager*			  GFieldManager;

#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "ServerPacketHandler.h"

enum Define
{
	DefaultKey = 0,
};