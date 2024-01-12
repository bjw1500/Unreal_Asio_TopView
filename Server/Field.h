#pragma once

/*
	필드는 여러개의 섹터들로 이루어져 있다.
	필드와 섹터를 어떻게 나누어줄까?

*/


class Field
{
public:
	Field(int32 fieldID);
	~Field();

	void Update(float deltaTime);


	//InGame
	void EnterPlayer(GameSessionRef session, int32 selectedCharacterID = Define::DefaultKey);
	void AddGameObject(GameObjectRef object, GameSessionRef execptSession = nullptr);
	void BroadCast(SendBufferRef packet, GameSessionRef execptSession = nullptr);

	int32 GetFieldID(){return _fieldID;}

private:
	int32 _fieldID;
	vector<FieldSectorRef> _fieldSectorList;

	unordered_map<ObectID, PlayerRef> _playerList;
	unordered_map<ObectID, GameObjectRef> _monsterList;
	unordered_map<ObectID, GameObjectRef> _objectList;

	atomic<int32> _objectId;
};

