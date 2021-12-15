#pragma once
#include "predef.h"
#include "chat/tsdeque.hpp"
#include "chat/message_wrapper.hpp"
#include "chat/tcp_connection.hpp"

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