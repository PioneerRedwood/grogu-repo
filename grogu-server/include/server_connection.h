#pragma once
#include "boost/asio.hpp"
#include "chat/tcp_connection.hpp"
#include "chat/tsdeque.hpp"
#include "chat/message_wrapper.hpp"

namespace chat {
//template<class T>
class ServerConnection
	: protected TcpConnection
{
	//using SharedMessage = std::shared_ptr<Message>;

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