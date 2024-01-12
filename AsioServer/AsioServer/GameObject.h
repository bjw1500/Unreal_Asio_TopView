#pragma once

class GameObject
{
public:
	GameObject();
	~GameObject();

	void SetObjectInfo(ObjectInfoRef objectInfo) {_objectInfo = objectInfo;}
	ObjectInfoRef GetObjectInfo(){return  _objectInfo;}


	FieldRef GetCurrentField(){return _currentField;}

private:
	ObjectInfoRef _objectInfo;
	FieldRef _currentField;

};

