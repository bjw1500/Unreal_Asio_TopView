#include "pch.h"
#include "ThreadManager.h"

/*------------------
	ThreadManager
-------------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitializeCriticalSection(&_criticalSection);
}

ThreadManager::~ThreadManager()
{
	DeleteCriticalSection(&_criticalSection);
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	EnterCriticalSection(&_criticalSection);

	_threads.push_back(thread([=]()
		{
			callback();
		}));

	LeaveCriticalSection(&_criticalSection);
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}
