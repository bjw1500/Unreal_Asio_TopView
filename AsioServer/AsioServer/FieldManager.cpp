#include "pch.h"
#include "FieldManager.h"
#include "Field.h"
#include <format>

FieldManager::FieldManager()
{
}

FieldManager::~FieldManager()
{
}

void FieldManager::Update(float deltaTime)
{
	for (auto fieldIter = _fieldList.begin(); fieldIter != _fieldList.end(); fieldIter++)
	{
		fieldIter->second->Update(deltaTime);
	}
}

FieldRef FieldManager::GetField(int field_ID)
{
	auto findIt = _fieldList.find(field_ID);

	//없는 필드 ID라면?
	if (findIt == _fieldList.end())
	{
		string serverInformation = format("잘못된 필드{} 입니다.", field_ID);
		return nullptr;
	}

	return findIt->second;
}

void FieldManager::AddField(int field_ID)
{
	FieldRef newField = make_shared<Field>(field_ID);
	_fieldList.insert(make_pair(field_ID, newField));
	std::string serverInformation = format("FieldID[{}]가 추가되었습니다.", field_ID);
	cout << serverInformation << endl;
}

void FieldManager::RemoveField(int field_ID)
{
}
