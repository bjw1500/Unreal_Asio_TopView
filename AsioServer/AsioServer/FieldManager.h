#pragma once

/*
	게임 필드를 관리해줄 매니저.

	필드는 여러개의 섹션들로 구성되어 있고,

	플레이어가 활동중인 섹션과 인접하지 않는 섹션들은 패킷을 보내지 않게 한다.

	필드 관리는 어떻게 해줄까?

	필드의 크기는 Unreal의 유닛 단위를 그대로 따라주자.

	일단 100 * 100을 넘어가면 넷컬링을 해줘보자.

*/


class FieldManager
{
public:
	FieldManager();
	~FieldManager();

	void Update(float deltaTime);

	FieldRef GetField(int field_ID);
	void AddField(int field_ID);
	void RemoveField(int field_ID);
	
private:
	unordered_map<int32, FieldRef> _fieldList;
};

