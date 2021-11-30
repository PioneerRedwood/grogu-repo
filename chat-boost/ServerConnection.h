#pragma once
#include "TcpConnection.h"

namespace chat {
// TODO: In server case, I/O handled by multi-threading.
// Hardly recommended, USE -- boost::asio::io_context::strand -- 
class ServerConnection : public TcpConnection
{
public:
	// TODO: required opening port
	ServerConnection(boost::asio::io_context& context, const int port);

	void Start();

private:

};

}