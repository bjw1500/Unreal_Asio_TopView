#pragma once
#include "Creature.h"
class Player : public Creature
{
public:
	Player();
	~Player();

	void							SetSession(GameSessionRef session){_session = session;}
	GameSessionRef GetSession(){return _session;}

private:
	GameSessionRef _session;

};

