#include "ClientConnection.h"

namespace chat {

void ClientConnection::Start()
{
	// Start async_connect to server
	socket_.async_connect(
		boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip_), port_),
		[&](std::error_code& error)->void
		{
			if (error) return;

			// TODO: notify to all about "gracefully" connected to server
			printf("connected server\n");

			Read();
		}
	);
}

template<typename T>
int ClientConnection::Send(T data) 
{

}

}