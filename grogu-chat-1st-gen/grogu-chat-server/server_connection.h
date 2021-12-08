#pragma once
#include "predef.h"
#include "common/chat.hpp"

namespace chat {
//template<class T>
class ServerConnection
	: protected TcpConnection
{
public:
	ServerConnection(
		boost::asio::io_context& context, TSDeque<chat::message::Message>& read_deque, 
		boost::asio::ip::tcp::socket socket);

	void Start();

	void Send(const chat::message::Message& msg) override;

private:


private:

};

}