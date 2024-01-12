#pragma once
#include "Session.h"

class Player;
class Account;

class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	void SetAccount(AccountRef account){_account = account;}

	GameSessionRef GetSessionRef() { return static_pointer_cast<GameSession> (shared_from_this()); }
	AccountRef				GetAccountRef(){return _account; }

private:
	AccountRef _account;

};