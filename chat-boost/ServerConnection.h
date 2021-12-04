#pragma once
#include "boost/asio.hpp"
#include "TSDeque.hpp"
#include "MessageWrapper.h"

namespace chat {
// TODO: In server case, I/O instructions be handled by multi thread.
// Hardly recommended, USE -- boost::asio::io_context::strand -- 
class ServerConnection
	: public std::enable_shared_from_this<ServerConnection>
{
public:
	ServerConnection(
		boost::asio::io_context& context, TSDeque<Message>& read_deque, 
		boost::asio::ip::tcp::socket socket);

	void Start();

	void Send(const Message& msg);
	void Send(const uint32_t _id, const std::string& _content);

	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }

private:
	void Read();

	void Write();

	void Store();

private:
	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;

	boost::asio::io_context::strand read_strand_;
	boost::asio::io_context::strand write_strand_;

	Message temp_;
	//Message write_msg_;

	// This data structure could decrease memory performance
	// Do not allocate and de-allocate! google-protobuf-msg
	TSDeque<Message>& read_deque_;
	TSDeque<Message> write_deque_;
};

}