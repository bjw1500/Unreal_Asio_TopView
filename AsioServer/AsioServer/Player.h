#pragma once
#include "Creature.h"
class Player : public Creature
{
public:
	Player();
	~Player();

	void							SetSession(ClientSessionRef session){_session = session;}
	ClientSessionRef GetSession(){return _session;}

private:
	ClientSessionRef _session;

};

