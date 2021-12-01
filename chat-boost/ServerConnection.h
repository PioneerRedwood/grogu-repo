#pragma once
#include "TcpConnection.h"
#include "TSDeque.h"

namespace chat {
// TODO: In server case, I/O instructions be handled by multi thread.
// Hardly recommended, USE -- boost::asio::io_context::strand -- 
class ServerConnection : public TcpConnection
{
public:
	ServerConnection(asio::io_context& context, TSDeque<SimpleMessage>& read_deque);

	void Start();

	void Send(const SimpleMessage& m);

private:


private:
	asio::io_context& context_;
	TSDeque<SimpleMessage>& read_deque_;
};

}