#include "TcpConnection.h"

namespace chat {
TcpConnection::TcpConnection(
	asio::io_context& context, TSDeque<SimpleMessage>& read_deque)
	//asio::io_context& context, tsdeque& read_deque)
	: context_(context), socket_(context), 
	read_strand_(context), write_strand_(context), read_deque_(read_deque)
{

}

void TcpConnection::Read() 
{
	std::cout << "Begin to read\n";
	asio::async_read(socket_,
		asio::buffer(&temp_, sizeof(SimpleMessage)),
			read_strand_.wrap([&](auto& error, size_t bytes)->void
				{
					// Handle error
					
					// Serialize and print into console
					auto show_msg = [](SimpleMessage& msg)->void {
						std::cout << "Serialize SimpleMessage to Console: "
							<< "[content: " << msg.content() << "]"
							<< "[owner_id: " << msg.owner_id() << "]"
							<< "[result: " << msg.result() << "]\n";
					};
					show_msg(temp_);

					// AND if this is important to keep on read_deque_? Store() ..!
					Store();
				}));
}

void TcpConnection::Write() 
{
	std::cout << "Begin to write\n";
	asio::async_write(socket_,
		// TODO: put the real sending data to the write_deque
		asio::buffer(&write_deque_.front(), sizeof(SimpleMessage)),
		write_strand_.wrap([&](auto& error, size_t bytes)->void 
			{
				// Handle error
				write_deque_.pop_front();

				// if there are more data to send? Call Write again
				if (!write_deque_.empty())
				{
					Write();
				}
			}));
}

void TcpConnection::Store()
{
	read_deque_.push_back(temp_);

	Read();
}

}