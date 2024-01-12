#pragma once
class FieldSector
{
public:
	FieldSector(int32 sectorID);
	 ~FieldSector();

	void Update(float deltaTime);

	int32 GetSectorID(){return _sectorID;}

private:
	int32 _sectorID;
};

