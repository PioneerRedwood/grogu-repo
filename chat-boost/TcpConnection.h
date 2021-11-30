#pragma once
#include <boost/asio.hpp>
#include <memory>
#include "ChattingMessage.pb.h"

namespace chat{
	namespace proto = shelby::chat::proto3;
	namespace asio = boost::asio;

// this class never be created by other class
// allowed only networking inherited instance class
class TcpConnection
	: std::enable_shared_from_this<TcpConnection>
{
protected:
	TcpConnection(boost::asio::io_context& context);

protected:
	// TODO: read data 
	void Read();

	// TODO: write data
	void Write();



protected:
	asio::io_context& context_;
	asio::ip::tcp::socket socket_;

	// 
	asio::io_context::strand read_strand_;
	asio::io_context::strand write_strand_;
	
	proto::SimpleChattingMessage temp_;
};
}