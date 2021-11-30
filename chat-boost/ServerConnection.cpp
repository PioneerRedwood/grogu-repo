#include "ServerConnection.h"

namespace chat{

ServerConnection::ServerConnection(boost::asio::io_context& context, const int port)
	: TcpConnection(context) {}

void ServerConnection::Start()
{
	// TODO: start accept and read

}



}