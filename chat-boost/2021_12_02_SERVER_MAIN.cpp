#include "SimpleChatServer.h"
#include "boost/asio.hpp"

int main()
{
	boost::asio::io_context context;
	chat::SimpleChatServer server{ context, 12190 };
	server.Start();

	context.run();

	return 0;
}