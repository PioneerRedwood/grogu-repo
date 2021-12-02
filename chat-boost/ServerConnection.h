#pragma once
#include "TcpConnection.h"
#include "TSDeque.h"
#include "ChattingMessage.pb.h"

namespace chat {
// TODO: In server case, I/O instructions be handled by multi thread.
// Hardly recommended, USE -- boost::asio::io_context::strand -- 
class ServerConnection : protected TcpConnection
{
public:
	ServerConnection(asio::io_context& context);

	void Start();

	void Send(const SimpleMessage& msg);


private:
	asio::io_context& context_;
	TSDeque<SimpleMessage> read_deque_;
	//tsdeque read_deque_;
};

}