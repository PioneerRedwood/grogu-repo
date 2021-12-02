/*
Developing Daily Record
2021-12-01
	Currently, it is not strictly enforced 
	handling an errors in Read() / Write().
*/

#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "ChattingMessage.pb.h"
#include "TSDeque.h"

namespace chat{
	using SimpleMessage = shelby::chat::proto3::SimpleChattingMessage;
	namespace asio = boost::asio;

// this class never be created by other class
// allowed only networking inherited instance class
class TcpConnection
	: std::enable_shared_from_this<TcpConnection>
{
protected:
	TcpConnection(asio::io_context& context, TSDeque<SimpleMessage>& read_deque);
	//TcpConnection(asio::io_context& context, tsdeque& read_deque);

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
	
	SimpleMessage temp_;
	TSDeque<SimpleMessage>& read_deque_;
	TSDeque<SimpleMessage> write_deque_;
	//tsdeque& read_deque_;
	//tsdeque write_deque_;
};
}