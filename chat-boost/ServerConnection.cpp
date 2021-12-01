#include "ServerConnection.h"

namespace chat{

ServerConnection::ServerConnection(asio::io_context& context, TSDeque<SimpleMessage>& read_deque)
	: TcpConnection(context, read_deque), context_(context), read_deque_(read_deque)
{

}

void ServerConnection::Start()
{
	Read();
}

void ServerConnection::Send(const SimpleMessage& msg)
{
	// TODO: start accept and read
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

}
