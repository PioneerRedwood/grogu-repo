#pragma once
#include "boost/asio.hpp"
#include "TSDeque.hpp"
#include "MessageWrapper.h"

namespace chat {
class ClientConnection 
	: public std::enable_shared_from_this<ClientConnection>
{
public:
	ClientConnection(boost::asio::io_context& context);

	void Start(const std::string& ip, const int port);

	void Send(const Message& msg);

private:
	void Read();

	void Write();

	void Store();

	void Connect(const std::string& ip, const int port);

	void TryConnect();

	void Ping();

private:
	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::io_context::strand read_strand_;
	boost::asio::io_context::strand write_strand_;

	boost::asio::steady_timer retry_timer_;
	boost::asio::steady_timer ping_timer_;

	Message temp_;
	std::string PING = "PING";
	TSDeque<Message> read_deque_;
	TSDeque<Message> write_deque_;
};
}