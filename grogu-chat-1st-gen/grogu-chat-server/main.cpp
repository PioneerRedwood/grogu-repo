#include "predef.h"
#include "simple_chatting_server.h"

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
