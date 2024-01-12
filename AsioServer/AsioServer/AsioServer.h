#pragma once

class ServerService : public enable_shared_from_this<ServerService>
{
public:
	ServerService(boost::asio::io_context& context, string host, string port);
	~ServerService();

	virtual bool Start();
	void				StartAccept();
	void				ProcessAccept(ClientSessionRef newSession, const boost::system::error_code& error);
	void				BroadCast();


	ClientSessionRef	CreateSession();
	void				AddSession(SessionRef session);
	void				RemoveSession(SessionRef session);
	int32				GetCurrentSessionCount() { return _sessions.size(); }
	boost::asio::io_context& GetContext(){return _context;}

private:
	boost::asio::io_context& _context;
	set<SessionRef> _sessions;
	tcp::acceptor _acceptor;

	string _host;
	string _port;
};

