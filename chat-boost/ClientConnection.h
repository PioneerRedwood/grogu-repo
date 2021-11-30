#pragma once
#include "TcpConnection.h"

namespace chat {
// TODO: Depends on how many handle the messages received from the server
// this class must handle only a communication for connection
class ClientConnection : public TcpConnection
{
public:
	// TODO: required connection ip, port
	ClientConnection(boost::asio::io_context& context, const std::string& ip, const int port)
		: TcpConnection(context), ip_(ip), port_(port) {}

	void Start();

	// TODO: Send data to server
	template<typename T>
	int Send(T data);

private:
	const std::string& ip_;
	int port_;
};
}