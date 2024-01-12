#include "pch.h"
#include "AsioClient.h"
#include "ThreadManager.h"
#include "ClientPacketHandler.h"

int main(int argc, char* argv[]) {

	boost::asio::io_context context;
	ClientServiceRef client = make_shared<ClientService>(context, "127.0.0.1", "7777");
	client->Start();
	GThreadManager->Join();

	return 0;
}
