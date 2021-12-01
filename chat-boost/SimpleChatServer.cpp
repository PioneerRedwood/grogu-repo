#include "SimpleChatServer.h"

namespace chat {
SimpleChatServer::SimpleChatServer(asio::io_context& context, const int port)
	: context_(context), acceptor_(context), 
	endpoint_(asio::ip::make_address("127.0.0.1"), port), accept_strand_(context)
{

}

void SimpleChatServer::Start()
{
	// TODO: Start accept client

	std::cout << "SimpleChatServer started ..\n";
	Accept();
}

void SimpleChatServer::Accept()
{
	// TODO: Accept new connection from the client
	acceptor_.async_accept(endpoint_,
		[&](std::error_code& error, asio::ip::tcp::socket socket)
		{
			// Handle if error occured

			// connected client info print
			std::stringstream ss;
			ss << socket.remote_endpoint();
			cout << "Connected " << ss.str() << "\n";

			// make shared_ptr of client
			std::shared_ptr<ServerConnection> conn =
				std::make_shared<ServerConnection>(
					context_, std::move(socket));

			// store client to proper data structure
			conn->Start();
			Accept();
		});
}
}