#include "pch.h"
#include "Session.h"
#include "ServerPacketHandler.h"
#include "GameSessionManager.h"
#include "AsioServer.h"


Session::Session(boost::asio::io_context& context, string& host, string& port)
	: _socket(context), 
	_recvBuffer(BUFFER_SIZE), 
	_resolver(context),
	_host(host),
	_port(port)
{

	InitializeCriticalSection(&_lock);
}

Session::~Session()
{
	DeleteCriticalSection(&_lock);
}



void Session::Start()
{
	//client로부터 연결됨
	cout << "Session Connected" << endl;

	//client 에서 날아온 데이터를 비동기적으로 읽는다.
	RegisterRecv();
}

void Session::Send(SendBufferRef sendBuffer)
{
	//bool registerSend = false;
	bool registerSend = true;

	//멀티쓰레드 환경을 위한 최적화.
	//누군가 패킷을 보내고 있는 중이라면 큐에 넣기만 한다.
	{
		EnterCriticalSection(&_lock);
		_sendQueue.push(sendBuffer);

		//이미 누군가 보내고 있는 중이라면 해당 쓰레드는 큐에 채우고 떠난다.
		registerSend = _sendRegistered.exchange(true) == false;
		LeaveCriticalSection(&_lock);
	}


	if (registerSend)
		RegisterSend();
	
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect(const string cause)
{
	if (_isConnected.exchange(false) == false)
		return;

	// TEMP
	cout << "Disconnect : " << cause << endl;

	OnDisconnected(); // ������ �ڵ忡�� ������
	GetService()->RemoveSession(GetSessionRef());

	RegisterDisconnect();
	_socket.close();
}

bool Session::RegisterConnect()
{
	if(IsConnected() == true)
		return false;

	//서버에 접속을 시도한다.

	boost::asio::async_connect(_socket, _resolver.resolve({ _host, _port }),
		boost::bind(&Session::ProcessConnect, shared_from_this(),
			_1));

	return true;
}

bool  Session::RegisterDisconnect()
{

	return true;
}

void  Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	_socket.async_read_some(boost::asio::buffer(_recvBuffer.WritePos(), _recvBuffer.FreeSize()), boost::bind(&Session::ProcessRecv, shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	//boost::asio::async_read(_socket, boost::asio::buffer(_recvBuffer.WritePos(), _recvBuffer.FreeSize()),
	//	boost::bind(&Session::ProcessRecv, shared_from_this(),
	//		boost::asio::placeholders::error,
	//		boost::asio::placeholders::bytes_transferred));
}

void  Session::RegisterSend()
{
	//연결이 안 되어 있다면 패킷을 보내지 않는다.
	if(IsConnected() == false)
		return;

	int32 writeSize = 0;
	{
		EnterCriticalSection(&_lock);
		//while (_sendQueue.empty() == false)
		//{
		//	SendBufferRef sendBuffer = _sendQueue.front();
		//	writeSize += sendBuffer->WriteSize();
		//	_sendQueue.pop();
		//	_sendList.push_back(sendBuffer);
		//}
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();
			writeSize += sendBuffer->WriteSize();
			boost::asio::async_write(_socket, boost::asio::buffer(sendBuffer->Buffer(), sendBuffer->WriteSize()),
				boost::bind(&Session::ProcessSend, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			_sendQueue.pop();

			//boost::asio::async_write(_socket, boost::asio::buffer(sendBuffer->Buffer(), sendBuffer->WriteSize()),
			//	[this](const boost::system::error_code& error, std::size_t bytes_transferred) {
			//		if (!error) {
			//			ProcessSend(error, bytes_transferred);
			//		}
			//		else {
			//			// 에러 처리
			//		}
			//	});
			//_sendQueue.pop();
	
		}



		LeaveCriticalSection(&_lock);
	}


}

void  Session::ProcessConnect(const boost::system::error_code& error)
{
	_isConnected.store(true);

	OnConnected();

	//연결되었으면 패킷을 받을 예약을 해준다.
	RegisterRecv();
}

void  Session::ProcessDisconnect(const boost::system::error_code& error)
{

}


//데이터를 받을 때.
void Session::ProcessRecv(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		cout << "ProcessRecv 에러 발생"<< endl;
		Disconnect("ProcessRecv 에러 발생");
		return;
	}

	if(bytes_transferred == 0)
		Disconnect("클라이언트 종료");

	//RecvBuffer 데이터를 넣는다.
	if (_recvBuffer.OnWrite((int32)bytes_transferred) == false)
	{
		Disconnect("OnWrite Overflow");
		return;
	}

	int32 dataSize = _recvBuffer.DataSize();
	int32 processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
	if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
	{
		Disconnect("OnRead Overflow");
		return;
	}

	_recvBuffer.Clean();
	//계속해서 데이터를 읽어준다.
	RegisterRecv();
}

//데이터를 보낼 때
void Session::ProcessSend(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if (!error) {
		std::cout << "Message sent successfully. bytes [" << bytes_transferred << std::endl;
	}
	else {
		std::cerr << "Write error: " << error.message() << std::endl;
	}

	_sendList.clear();
	OnSend(bytes_transferred);
	bool registerSend = false;
	{
		//보내고 보니까 SendQueue가 비어 있다면, 여기서 끝내고.
		//만약 SendQueue에 패킷이 남아 있다면 보내던 쓰레드가 끝까지 보내준다.
		EnterCriticalSection(&_lock);
		if (_sendQueue.empty())
			_sendRegistered.store(false);
		else
			registerSend = true;
		LeaveCriticalSection(&_lock);
	}

	if (registerSend)
		RegisterSend();

}

int32 Session::OnRecv(BYTE* buffer, int32 len)
{
	int32 processLen = 0;

	//패킷이 완성된 상태로 왔는지 검증하는 구간.
	while (true)
	{
		int32 dataSize = len - processLen;
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		if (dataSize < header.size)
			break;
		OnRecvPacket(&buffer[processLen], header.size);

		processLen += header.size;
	}

	return processLen;

}

ClientSession::ClientSession(boost::asio::io_context& context, string& host, string& port)
	: Session(context, host, port)
{
}

ClientSession::~ClientSession()
{
}

void ClientSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<ClientSession>(shared_from_this()));

	cout << "새로운 플레이어 서버 연결" << endl;

	Send(ServerPacketHandler::Make_S_Connect());
}

void ClientSession::OnDisconnected()
{
	//if (_myPlayer != nullptr)
	//{
	//	_myPlayer->Disconnect();
	//}

	GSessionManager.Remove(static_pointer_cast<ClientSession>(shared_from_this()));

}

void ClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	SessionRef session = GetSessionRef();
	PacketHeader header = *((PacketHeader*)buffer);
	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void ClientSession::OnSend(int32 len)
{

}