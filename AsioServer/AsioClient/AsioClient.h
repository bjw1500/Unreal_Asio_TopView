#pragma once

class ClientService : public enable_shared_from_this<ClientService>
{
public:
	ClientService(boost::asio::io_context& context, string host, string port);
	~ClientService();

	virtual bool					Start();
	ServerSessionRef		CreateSession();
	void								AddSession(SessionRef session);
	void								RemoveSession(SessionRef session);
	int32								GetCurrentSessionCount() { return _sessions.size(); }

protected:
	boost::asio::io_context& _context;

	set<SessionRef> _sessions;

	string _host;
	string _port;
};



