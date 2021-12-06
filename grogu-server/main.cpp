#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif 

#include <iostream>
#include "boost/asio.hpp"

#include "include/simple_chatting_server.h"
#include "chat/message_wrapper.hpp"

int main()
{
	namespace asio = boost::asio;
	using namespace chat;

	asio::io_context context;
	SimpleChatServer server{ context, 12190, 1 };
	server.Start();

	while (true)
	{
		server.Tick(-1, false);
}
	return 0;
}
