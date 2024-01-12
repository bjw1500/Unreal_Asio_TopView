#include "pch.h"
#include "Field.h"
#include "FieldSector.h"
#include "GameObject.h"
#include "Creature.h"
#include "Player.h"
#include "GameSession.h"

Field::Field(int32 fieldID)
	: _fieldID(fieldID)
{
}

Field::~Field()
{
}

void Field::Update(float deltaTime)
{
	for (FieldSectorRef fieldSector : _fieldSectorList)
	{
		fieldSector->Update(deltaTime);
	}

}

void Field::EnterPlayer(GameSessionRef session, int32 selectedCharacterID)
{
	//플레이어를 입장시킨다.

	ObjectInfoRef playerInfo = make_shared<Protocol::ObjectInfo>();
	playerInfo->set_objectid(_objectId++);
	Protocol::Position* currentPos = new Position(); 
	playerInfo->mutable_position()->CopyFrom(*currentPos);
	playerInfo->set_type(ObjectType::PLAYER);

	GameObjectRef player = make_shared<GameObject>();
	player->SetObjectInfo(playerInfo);


	//플레이어 입장
	{
		Protocol::S_EnterField pkt;
		pkt.mutable_objectinfo()->CopyFrom(*playerInfo);
		SendBufferRef buffer = ServerPacketHandler::MakeSendBuffer(pkt, Protocol::S_ENTER_FIELD);
		session->Send(buffer);
		cout << "새로운 플레이어 입장"<< endl;
	}


	//기존 필드에 있던 몬스터와 플레이어의 정보를 보내준다.
	{


	}


	AddGameObject(player, session->GetSessionRef());
}

void Field::AddGameObject(GameObjectRef object, GameSessionRef execptSession)
{
	ObjectType type = object->GetObjectInfo()->type();

	switch (type)
	{
	case Protocol::PLAYER:
		break;
	case Protocol::MONSTER:
		break;
	case Protocol::ITEM:
		break;
	case Protocol::UNKNOWN:
		break;
	default:
		break;
	}
	
	//같은 필드에 있는 다른 플레이어들에게 패킷을 보낸다.
	{


	}
}

void Field::BroadCast(SendBufferRef packet, GameSessionRef execptSession)
{
	for (auto& player : _playerList)
	{
		GameSessionRef session = player.second->GetSession();

		if(execptSession == session)
			continue;

		session->Send(packet);
	}
}
