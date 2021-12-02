#pragma once
#include "TcpConnection.h"
#include "TSDeque.h"
#include "ChattingMessage.pb.h"

namespace chat {
// TODO: Depends on how many handle the messages received from the server
// this class must handle only a communication for connection
class ClientConnection : public TcpConnection
{
public:
	// TODO: required connection ip, port
	ClientConnection(asio::io_context& context);

	void Start(const std::string& ip, const int port);

	// TODO: Send data to server
	void Send(const SimpleMessage& msg);

private:
	void Connect(const std::string& ip, const int port);

	void Ping();

private:
	asio::io_context& context_;
	asio::steady_timer ping_timer_;
	//tsdeque read_deque_;
	TSDeque<SimpleMessage> read_deque_;


};
}