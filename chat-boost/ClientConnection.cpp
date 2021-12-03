#include "ClientConnection.h"
#include "MessageWrapper.h"

namespace chat {


ClientConnection::ClientConnection(asio::io_context& context)
	: context_(context), read_strand_(context), write_strand_(context),
	socket_(context),
	ping_timer_(context, chrono::seconds(1)),
	retry_timer_(context, chrono::seconds(1))
{}

void ClientConnection::Start(const std::string& ip, const int port)
{
	std::cout << "Client Started\n";
	if (!socket_.is_open())
	{
		TryConnect();
	}
}

void ClientConnection::Read()
{
	std::cout << "Begin to read.. " << read_deque_.size() << "\n";
	asio::async_read(socket_,
		asio::buffer(&temp_, sizeof(SimpleMessage)),
		read_strand_.wrap([&](auto& error, size_t bytes)->void
			{
				// Handle error
				if (error)
				{
					cerr << "Error: read " << error << "\n";
					return;
				}

				ShowPrettyMessage(temp_);

				// AND if this is important to keep on read_deque_? Store() ..!
				Store();
			}));
}

void ClientConnection::Write()
{
	std::cout << "Begin to write .." << write_deque_.size() << "\n";
	
	asio::async_write(socket_,
		// TODO: put the real sending data to the write_deque
		asio::buffer(&write_deque_.front(), sizeof(SimpleMessage)),
		write_strand_.wrap([&](auto& error, size_t bytes)->void
			{
				// Handle error
				if (error)
				{
					cerr << "Error: write " << error << "\n";
					return;
				}

				// if there are more data to send? Call Write again
				if (!write_deque_.empty())
				{
					write_deque_.pop_front();
				}
			}));
}

void ClientConnection::Store()
{
	read_deque_.push_back(temp_);

	Read();
}

void ClientConnection::Connect(const std::string& ip, const int port)
{
	socket_.async_connect(
		asio::ip::tcp::endpoint(asio::ip::make_address(ip), port),
		[this](auto& error)
		{
			if (error) 
			{
				// TODO: try connect every time
				TryConnect();
				return;
			}

			// TODO: notify to all about "gracefully" connected to server
			std::cout << "Connected gracefully ..\n";

			Ping();
			Read();
		}
	);
}

void ClientConnection::TryConnect()
{
	retry_timer_.expires_at(retry_timer_.expiry() + std::chrono::seconds(1));
	retry_timer_.async_wait(
		[this](auto& error)
		{
			if (error && socket_.is_open())
			{
				return;
			}

			std::cout << "TryConnect .. \n";
			Connect("127.0.0.1", 12190);
		});

}

void ClientConnection::Send(const SimpleMessage& msg)
{
	asio::post(context_, [&, msg]()->void
		{
			bool isWriting = !write_deque_.empty();
			write_deque_.push_back(msg);
			if (!isWriting)
			{
				Write();
			}
		});
}

void ClientConnection::Ping()
{
	ping_timer_.expires_at(ping_timer_.expiry() + chrono::seconds(1));
	ping_timer_.async_wait([&](auto& error)->void
		{
			if (error)
			{
				return;
			}

			SimpleMessage msg;
			msg.set_content("PING");
			msg.set_owner_id(INT32_MAX);
			msg.set_result(0);

			ShowPrettyMessage(msg);

			Send(std::move(msg));
			Ping();
		});

}
}