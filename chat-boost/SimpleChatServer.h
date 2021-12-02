#pragma once
#include "boost/asio.hpp"
#include "ServerConnection.h"

namespace chat {
class SimpleChatServer
{
public:
	explicit SimpleChatServer(asio::io_context& context, const int port);

	void Start();
	
private:
	void Accept();

private:
	asio::io_context& context_;
	asio::io_context::strand accept_strand_;
	asio::ip::tcp::acceptor acceptor_;
	asio::ip::tcp::endpoint endpoint_;
};
}