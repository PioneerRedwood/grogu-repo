/*
Developing Daily Record
2021-12-01
	Currently, it is not strictly enforced 
	handling an errors in Read() / Write().
2021-12-03
	Currently, this class is deprecated.
	Define, what going to use
*/

#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "ChattingMessage.pb.h"
#include "TSDeque.hpp"

namespace chat{
	namespace asio = boost::asio;
class TcpConnection
	: std::enable_shared_from_this<TcpConnection>
{
protected:
	TcpConnection(
		asio::io_context& context, 
		TSDeque<Message>& read_deque, 
		asio::ip::tcp::socket socket);

protected:
	void Read();

	void Write();

	void Store();

	asio::ip::tcp::socket& GetSocket() { return socket_; }

protected:
	asio::io_context& context_;
	asio::ip::tcp::socket socket_;

	asio::io_context::strand read_strand_;
	asio::io_context::strand write_strand_;
	
	Message temp_;
	TSDeque<Message>& read_deque_;
	TSDeque<Message> write_deque_;
};
}