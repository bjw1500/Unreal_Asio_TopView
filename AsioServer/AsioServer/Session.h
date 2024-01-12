#pragma once

#include "SendBuffer.h"
#include "RecvBuffer.h"

struct PacketHeader
{
	uint16 size;
	uint16 id; 
};

class Session : public enable_shared_from_this<Session>
{
public:
	Session(boost::asio::io_context& context, string& host, string &port);
	~Session();

	void							Start();

	void							Send(SendBufferRef sendBuffer);
	bool							Connect();
	void							Disconnect(const string cause);


	tcp::socket&			GetSocket() { return _socket; };
	ServiceRef				GetService(){return _server;}
	void							SetService(ServiceRef server){_server = server;}
	SessionRef				GetSessionRef(){return static_pointer_cast<Session>(shared_from_this());}

	bool							IsConnected(){return _isConnected;}
	bool							RegisterConnect();
	bool							RegisterDisconnect();
	void							RegisterRecv();
	void							RegisterSend();

	void							ProcessConnect(const boost::system::error_code& error);
	void							ProcessDisconnect(const boost::system::error_code& error);
	void							ProcessRecv(const boost::system::error_code& error, size_t bytes_transferred);
	void							ProcessSend(const boost::system::error_code& error, std::size_t bytes_transferred);

	//컨테츠단을 위한 이벤트 함수들.
	int32							OnRecv(BYTE* buffer, int32 len);
	virtual void				OnRecvPacket(BYTE* buffer, int32 len){}

	virtual void				OnConnected() { }
	virtual void				OnSend(int32 len) { }
	virtual void				OnDisconnected() { }
private:

	tcp::socket							_socket;
	string										_host;
	string										_port;
	boost::asio::ip::tcp::resolver _resolver;
	ServiceRef							_server;

	queue<SendBufferRef> _sendQueue;
	vector<SendBufferRef> _sendList;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};
	RecvBuffer	_recvBuffer;

	CRITICAL_SECTION _lock;
	atomic<bool>			_isConnected = false;
	atomic<bool>			_sendRegistered = false;
};


class ClientSession : public Session
{
public:
	ClientSession(boost::asio::io_context& context, string& host, string& port);
	~ClientSession();
	ClientSessionRef	GetSessionRef() { return static_pointer_cast<ClientSession>(shared_from_this()); }
	virtual void				OnRecvPacket(BYTE* buffer, int32 len);
	virtual void				OnConnected();
	virtual void				OnSend(int32 len);
	virtual void				OnDisconnected();

	void							SetAccount(AccountRef account) { _account = account; }
	AccountRef				GetAccountRef() { return _account; }

private:
	AccountRef _account;
};