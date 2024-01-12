#include "pch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include <format>


atomic<int32> _objectId = 1;
GameRoomRef GRoom = make_shared<GameRoom>();


GameRoom::GameRoom()
{
	//PlayerRef newPlayer = CreatePlayer();
	//AddObject(newPlayer, NULL);
	cout << "GameRoom 생성" << endl;
}

GameRoom::~GameRoom()
{

	cout << "GameRoom 삭제" << endl;

}

void GameRoom::Update(float deltaTime)
{


}
