#pragma once
#include "boost/asio.hpp"
#include "TcpConnection.h"
#include "TSDeque.h"
#include "ChattingMessage.pb.h"

/*
2021-12-03
it seems that ServerConnection and CliecntConnection 
do not have common points. 

*/

namespace chat {
// TODO: In server case, I/O instructions be handled by multi thread.
// Hardly recommended, USE -- boost::asio::io_context::strand -- 
class ServerConnection
	: public std::enable_shared_from_this<ServerConnection>
{
public:
	ServerConnection(
		asio::io_context& context, TSDeque<SimpleMessage>& read_deque, 
		asio::ip::tcp::socket socket);

	void Start();

	void Send(const SimpleMessage& msg);

	asio::ip::tcp::socket& GetSocket() { return socket_; }

private:
	void Read();

	void Write();

	void Store();

private:
	asio::io_context& context_;
	asio::ip::tcp::socket socket_;

	asio::io_context::strand read_strand_;
	asio::io_context::strand write_strand_;


	SimpleMessage temp_;
	TSDeque<SimpleMessage>& read_deque_;
	TSDeque<SimpleMessage> write_deque_;
};

}