#pragma once


class GameSessionManager
{
public:
	GameSessionManager();
	~GameSessionManager();
public:
	void Add(ClientSessionRef session);
	void Remove(ClientSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

private:
	CRITICAL_SECTION _criticalSection;
	set<ClientSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
