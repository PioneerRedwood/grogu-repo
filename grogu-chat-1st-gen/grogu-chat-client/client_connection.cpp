#include "client_connection.h"

namespace chat {

ClientConnection::ClientConnection(boost::asio::io_context& context)
	: TcpConnection(context, read_deque_),
	ping_timer_(context, chrono::seconds(1)),
	retry_timer_(context, chrono::seconds(1))
{
	
}

void ClientConnection::Start(const std::string& ip, const int port)
{
	std::cout << "Client Started\n";
	Connect("127.0.0.1", 12190);

	//RetryConnect();
}

void ClientConnection::Connect(const std::string& ip, const int port)
{
	socket_.async_connect(
		boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip), port),
		[this](auto& error)
		{
			if (error)
			{
				// TODO: try connect every time
				RetryConnect();
				return;
			}

			// TODO: notify to all about "gracefully" connected to server
			std::cout << "Connected gracefully ..\n";

			Ping();
			ReadHeader();
		}
		);
}

void ClientConnection::RetryConnect()
{
	retry_timer_.expires_at(retry_timer_.expiry() + std::chrono::seconds(1));
	retry_timer_.async_wait(
		[this](auto& error)
		{
			if (error && socket_.is_open())
			{
				return;
			}
			else
			{
				std::cout << "TryConnect .. \n";
				Connect("127.0.0.1", 12190);
			}
		});

}

// copy 
void ClientConnection::Send(const chat::message::Message& msg)
{
	boost::asio::post(context_, [this, data = std::move(msg)]()->void
	{
		bool isWriting = !write_deque_.empty();
		write_deque_.push_back(data);
		if (!isWriting)
		{
			WriteHeader();
		}
	});
}

void ClientConnection::Ping()
{
	ping_timer_.expires_at(ping_timer_.expiry() + std::chrono::seconds(1));
	ping_timer_.async_wait([this](auto& error)->void
		{
			if (error)
			{
				return;
			}

			std::cout << "Ping()\n";

			Ping();
		});

}
}