#include "SimpleChatServer.h"
#include "boost/asio.hpp"

int server_main()
{
	boost::asio::io_context context;
	chat::SimpleChatServer server{ context, 12190 };
	server.Start();
	context.run();

	while (true)
	{
		server.Tick(-1, false);
	}
	
	return 0;
}