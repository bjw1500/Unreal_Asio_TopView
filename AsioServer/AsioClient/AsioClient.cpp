#include "pch.h"
#include "AsioClient.h"
#include "ClientPacketHandler.h"
#include "Session.h"
#include "ThreadManager.h"

unique_ptr<ThreadManager> GThreadManager = nullptr;

ClientService::ClientService(boost::asio::io_context& context, string host, string port)
	: _context(context),
	_host(host),
	_port(port)
{

}

ClientService::~ClientService()
{
}

bool ClientService::Start()
{
	ServerSessionRef session = CreateSession();
	session->SetService(shared_from_this());
	PacketHandler::Init();

	if(session->Connect() == false)
		return false;

	GThreadManager = make_unique<ThreadManager>();
	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				_context.run();
			});
	}

	return true;
}

ServerSessionRef  ClientService::CreateSession()
{
	ServerSessionRef session = make_shared<ServerSession>(_context, _host, _port);
	session->SetService(shared_from_this());
	return session;
}

void ClientService::AddSession(SessionRef session)
{
	_sessions.insert(session);
}

void ClientService::RemoveSession(SessionRef session)
{
	_sessions.erase(session);
}

