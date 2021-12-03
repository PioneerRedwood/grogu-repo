#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif 

#include <iostream>
#include "boost/asio.hpp"

// uncomment ON / comment OFF what you want to build
//#define CLIENT true
#define SERVER true

#ifdef CLIENT
#include "ClientConnection.h"
#endif 

#ifdef SERVER
#include "SimpleChatServer.h"
#endif

int main()
{
	namespace asio = boost::asio;

#ifdef CLIENT
	asio::io_context context;
	chat::ClientConnection client{context};
	client.Start("127.0.0.1", 12190);
	context.run();

	context.stop();
#endif

#if SERVER
	asio::io_context context;
	chat::SimpleChatServer server{ context, 12190, 1 };
	
	server.Start();

	while (true)
	{
		server.Tick(-1, false);
	}

#endif

	return 0;
}
