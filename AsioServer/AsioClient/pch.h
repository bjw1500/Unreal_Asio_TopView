#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <math.h>
#include <functional>
#include <string>
#include <mutex>


#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "Protocol.pb.h"

#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")

#ifdef _DEBUG
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif


extern std::unique_ptr<class ThreadManager> GThreadManager;
using SessionRef = std::shared_ptr<class Session>;
using ServiceRef = std::shared_ptr<class ClientService>;
using SendBufferRef = std::shared_ptr<class SendBuffer>;
using ClientServiceRef = std::shared_ptr<class ClientService>;

using ServerSessionRef = std::shared_ptr<class ServerSession>;
using ClientSessionRef = std::shared_ptr<class ClientSession>;
using boost::asio::ip::tcp;
using namespace std;

using GameRoomRef = std::shared_ptr<class GameRoom>;
using GameObjectRef = std::shared_ptr<class GameObject>;
using PlayerRef = std::shared_ptr<class Player>;
using MonsterRef = std::shared_ptr<class Monster>;
using CreatureRef = std::shared_ptr<class Creature>;
using AccountRef = std::shared_ptr<class Account>;
using FieldRef = shared_ptr<class Field>;
using FieldSectorRef = shared_ptr<class FieldSector>;
using ObjectInfoRef = shared_ptr<Protocol::ObjectInfo>;



using namespace Protocol;

extern class DBConnectionPool* GDBConnectionPool;
extern class FieldManager* GFieldManager;

/////////////#Define
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using CharacterKey = int32;
using ObectID = int32;

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}