#include "ServerConnection.h"

namespace chat{

ServerConnection::ServerConnection(asio::io_context& context)
	: TcpConnection(context, read_deque_), context_(context)
{

}

void ServerConnection::Start()
{
	Read();
}

void ServerConnection::Send(const SimpleMessage& msg)
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

}
