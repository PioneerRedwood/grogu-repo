#include "ClientConnection.h"

namespace chat {


ClientConnection::ClientConnection(asio::io_context& context)
	: TcpConnection(context, read_deque_), 
	context_(context), ping_timer_(context, chrono::seconds(1))
{}

void ClientConnection::Start(const std::string& ip, const int port)
{
	Connect(ip, port);
}

void ClientConnection::Connect(const std::string& ip, const int port)
{
	socket_.async_connect(
		asio::ip::tcp::endpoint(asio::ip::make_address(ip), port),
		[this](auto& error)
		{
			if (error) return;

			// TODO: notify to all about "gracefully" connected to server
			printf("connected server\n");

			Read();
		}
	);
}

void ClientConnection::Send(const SimpleMessage& msg)
{
	asio::post(context_, write_strand_.wrap([&, msg]()->void
		{
			bool isWriting = !write_deque_.empty();
			write_deque_.push_back(msg);
			if (!isWriting)
			{
				Write();
			}
		}));
}

void ClientConnection::Ping()
{
	//timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
	//timer_.async_wait(boost::bind(&printer::print, this));

	ping_timer_.expires_at(ping_timer_.expiry() + chrono::seconds(1));
	ping_timer_.async_wait([&](auto& error)->void
		{
			SimpleMessage msg;
			msg.set_content("PING");
			msg.set_owner_id(INT32_MAX);
			msg.set_result(0);

			Send(msg);
		});

}
}