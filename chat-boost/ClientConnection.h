#pragma once
#include "TcpConnection.h"
#include "TSDeque.h"
#include "ChattingMessage.pb.h"

namespace chat {
// TODO: Depends on how many handle the messages received from the server
// this class must handle only a communication for connection
class ClientConnection 
	: public std::enable_shared_from_this<ClientConnection>
{
public:
	// TODO: required connection ip, port
	ClientConnection(asio::io_context& context);

	void Start(const std::string& ip, const int port);

	// TODO: Send data to server
	void Send(const SimpleMessage& msg);

private:
	void Read();

	void Write();

	void Store();

	void Connect(const std::string& ip, const int port);

	void TryConnect();

	void Ping();

private:
	asio::io_context& context_;
	asio::ip::tcp::socket socket_;
	asio::io_context::strand read_strand_;
	asio::io_context::strand write_strand_;

	asio::steady_timer retry_timer_;
	asio::steady_timer ping_timer_;

	SimpleMessage temp_;
	TSDeque<SimpleMessage> read_deque_;
	TSDeque<SimpleMessage> write_deque_;
};
}