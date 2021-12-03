#include "ServerConnection.h"
#include "MessageWrapper.h"

namespace chat{

ServerConnection::ServerConnection(
	asio::io_context& context, TSDeque<SimpleMessage>& read_deque, asio::ip::tcp::socket socket)
	: context_(context), read_strand_(context), write_strand_(context),
	socket_(std::move(socket)), read_deque_(read_deque)
{}

void ServerConnection::Start()
{
	Read();
}

void ServerConnection::Send(const SimpleMessage& msg)
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

void ServerConnection::Read()
{
	std::cout << "Begin to read\n";
	asio::async_read(socket_,
		asio::buffer(&temp_, sizeof(SimpleMessage)),
		//asio::buffer(&temp_, 12),
		read_strand_.wrap([&](auto& error, size_t bytes)->void
			{
				// Handle error
				if (error)
				{
					cerr << "Error: read " << error << "\n";
					return;
				}
				
				std::cout << "READ (size:" << bytes << ") => ";
				//ShowPrettyMessage(temp_);
				if (std::string str; google::protobuf::TextFormat::PrintToString(temp_, &str))
				{
					std::cout << str << "\n";
				}

				// AND if this is important to keep on read_deque_? Store() ..!
				Store();
			}));
}

void ServerConnection::Write()
{
	std::cout << "Begin to write\n";
	asio::async_write(socket_,
		asio::buffer(&write_deque_.front(), sizeof(SimpleMessage)),
		write_strand_.wrap([&](auto& error, size_t bytes)->void
			{
				// Handle error
				if (error)
				{
					cerr << "Error: read " << error << "\n";
					return;
				}

				// if there are more data to send? Call Write again
				if (!write_deque_.empty())
				{
					write_deque_.pop_front();
				}
			}));
}

void ServerConnection::Store()
{
	read_deque_.push_back(temp_);

	Read();
}

}
