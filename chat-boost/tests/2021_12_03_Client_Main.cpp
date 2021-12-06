#include <iostream>
#include "boost/asio.hpp"
#include "ClientConnection.h"

int client_main()
{
	namespace asio = boost::asio;
	asio::io_context context;
	chat::ClientConnection conn{ context };
	conn.Start("127.0.0.1", 12190);
	
	context.run();

	return 0;
}