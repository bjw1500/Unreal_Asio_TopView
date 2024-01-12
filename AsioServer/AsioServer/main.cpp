#include "pch.h"
#include "AsioServer.h"
#include "ThreadManager.h"

int main(int argc, char* argv[])
{
	boost::asio::io_context  context;
	ServiceRef service = make_shared<ServerService>(context, "127.0.0.1", "7777");
	service->Start();

	while (true)
	{
		this_thread::sleep_for(1s);
		service->BroadCast();

	}

	GThreadManager->Join();
	return 0;
}