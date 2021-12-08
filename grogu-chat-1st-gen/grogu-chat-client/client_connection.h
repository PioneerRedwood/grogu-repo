#pragma once
#include "predef.h"
#include "chat/tcp_connection.hpp"
#include "tsdeque.hpp"
#include "message_wrapper.hpp"

namespace chat {
class ClientConnection
	: protected TcpConnection
{
	
public:
	ClientConnection(boost::asio::io_context& context);

	void Start(const std::string& ip, const int port);

	//void Send(const SharedMessage& msg);
	void Send(const chat::message::Message& msg) override;

private:
	void Connect(const std::string& ip, const int port);

	void RetryConnect();

	void Ping();

private:
	boost::asio::steady_timer retry_timer_;
	boost::asio::steady_timer ping_timer_;

	chat::message::Message write_msg_;
};
}