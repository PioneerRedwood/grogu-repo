#include "server_connection.h"

namespace chat{

ServerConnection::ServerConnection(
	boost::asio::io_context& context, TSDeque<chat::message::Message>& read_deque, boost::asio::ip::tcp::socket socket)
	: TcpConnection(context, std::move(socket), read_deque)
{}

void ServerConnection::Start()
{
	ReadHeader();
}

void ServerConnection::Send(const chat::message::Message& msg)
{
	// copy?
	boost::asio::post(context_, [this, data = msg]()->void
	{
		bool isWriting = !write_deque_.empty();
		write_deque_.push_back(data);
		if (!isWriting)
		{
			WriteHeader();
		}
	});
}

}
