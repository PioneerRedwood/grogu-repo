#include "predef.h"
#include "client_connection.h"

int main()
{
	boost::asio::io_context context;
	chat::ClientConnection client{ context };
	client.Start("127.0.0.1", 12190);
	context.run();

	context.stop();
	
	return 0;
}
