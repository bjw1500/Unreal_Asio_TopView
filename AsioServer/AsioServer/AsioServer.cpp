#include "pch.h"
#include "AsioServer.h"
#include "Session.h"
#include "ThreadManager.h"
#include "ServerPacketHandler.h"

unique_ptr<ThreadManager> GThreadManager = nullptr;

ServerService::ServerService(boost::asio::io_context& context, string host, string port)
		: _context(context),
		_acceptor(_context, tcp::endpoint(tcp::v4(), std::stoi(port))),
		_host(host),
		_port(port)
{
	
}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	/*
	* TODO
		서버 구동에 필요한 과정들을 넣어준다.
	*/

	//서버가 시작하였으면 클라이언트를 받을 준비를 해준다.
	ServerPacketHandler::Init();
	GThreadManager = make_unique<ThreadManager>();
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				_context.run();
			});
	}

	StartAccept();
	return true;
}

void ServerService::StartAccept()
{
		SessionRef newSession = CreateSession();
		_acceptor.async_accept(newSession->GetSocket(),
			boost::bind(&ServerService::ProcessAccept, this, newSession,
				boost::asio::placeholders::error));
}


void ServerService::ProcessAccept(SessionRef newSession, const boost::system::error_code& error)
{
		if (!error)
		{
			newSession->ProcessConnect(error);
			AddSession(newSession);
		}
		else
		{
			cout << "Process Aceept Error." << endl;
		}

		//실패하든, 성공하든 다시 Accept 준비를 해준다.
		StartAccept();
}

void ServerService::BroadCast()
{
	
	for (auto& session : _sessions)
	{
		session->Send(ServerPacketHandler::Make_S_Connect());
	}
	
	
}

SessionRef ServerService::CreateSession()
{
	SessionRef session = make_shared<ClientSession>(_context, _host, _port);
	session->SetService(shared_from_this());
	return session;
}

void ServerService::AddSession(SessionRef session)
{
	_sessions.insert(session);
}

void ServerService::RemoveSession(SessionRef session)
{
	_sessions.erase(session);
}

