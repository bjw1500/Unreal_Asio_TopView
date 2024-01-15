#include "pch.h"
#include "GameSessionManager.h"
#include "Session.h"

GameSessionManager GSessionManager;

GameSessionManager::GameSessionManager()
{
	InitializeCriticalSection(&_criticalSection);
}

GameSessionManager::~GameSessionManager()
{
	DeleteCriticalSection(&_criticalSection);
}

void GameSessionManager::Add(ClientSessionRef session)
{
	EnterCriticalSection(&_criticalSection);
	_sessions.insert(session);
	LeaveCriticalSection(&_criticalSection);
}

void GameSessionManager::Remove(ClientSessionRef session)
{
	EnterCriticalSection(&_criticalSection);
	_sessions.erase(session);
	
	LeaveCriticalSection(&_criticalSection);
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	EnterCriticalSection(&_criticalSection);
	for (ClientSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
	LeaveCriticalSection(&_criticalSection);
}