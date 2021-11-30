#include "TcpConnection.h"

namespace chat {
TcpConnection::TcpConnection(boost::asio::io_context& context)
	: context_(context), socket_(context),
	read_strand_(context), write_strand_(context)
{

}

void TcpConnection::Read() {}

void TcpConnection::Write() {}

}