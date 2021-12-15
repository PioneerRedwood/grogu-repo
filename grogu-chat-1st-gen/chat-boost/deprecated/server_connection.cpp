#include "server_connection.h"

namespace chat{

ServerConnection::ServerConnection(
	boost::asio::io_context& context, TSDeque<Message>& read_deque, boost::asio::ip::tcp::socket socket)
	: context_(context), read_strand_(context), write_strand_(context),
	socket_(std::move(socket)), read_deque_(read_deque)
{}

void ServerConnection::Start()
{
	ReadHeader();
}

void ServerConnection::Send(const Message& msg)
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

void ServerConnection::Send(const uint32_t _id, const std::string& _content)
{}

void ServerConnection::ReadHeader()
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(&read_msg_.header_, sizeof(header)),
		read_strand_.wrap([this](auto& error, size_t bytes)->void
		{
			if (error)
			{
				cerr << "Error: ReadHeader() " << error << "\n";
				return;
			}
			else
			{
				if (read_msg_.header_.size() > 0)
				{
					ReadBody(read_msg_.header_.size());
				}
			}

			ReadHeader();
		}));
}

void ServerConnection::ReadBody(std::size_t size)
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(&read_msg_.body_, size),
		read_strand_.wrap([this](auto& error, size_t bytes)->void
		{
			if (error)
			{
				cerr << "Error: ReadBody() " << error << "\n";
				return;
			}
			else
			{
				Store();
				return;
			}
		}));
}

void ServerConnection::WriteHeader()
{
	boost::asio::async_write(socket_,
		boost::asio::buffer(&write_deque_.front().header_, sizeof(header)),
		write_strand_.wrap([this](auto& error, size_t bytes)->void
	{
		if (error)
		{
			cerr << "Error: WriteHeader() " << error << "\n";
			return;
		}
		else
		{
			if (write_deque_.front().header_.size() > 0)
			{
				WriteBody(write_deque_.front().header_.size());
			}

			// if there are more data to send? Call Write again
			//if (!write_deque_.empty())
			//{
			//	cout << "header: " << write_deque_.front()->header_.size() << " " << write_deque_.front()->header_.id() << "\n";
			//	cout << "body: " << write_deque_.front()->body_.content() << "\n";
			//	write_deque_.pop_front();
			//}
		}
	}));
}

void ServerConnection::WriteBody(std::size_t size)
{
	boost::asio::async_write(socket_,
		boost::asio::buffer(&write_deque_.front().body_, size),
		write_strand_.wrap([this, header_size = size](auto& error, size_t bytes)->void
		{
			if (error)
			{
				cerr << "Error: WriteHeader() " << error << "\n";
				return;
			}
			else
			{
				// look at what it wrote
				cout << "Write completed[ "
					<< "header; " << header_size << "bytes\t"
					<< "body; " << bytes << "bytes ]\n";

			}
		}));
}

void ServerConnection::Store()
{
	// if use temp_.PrintDebugString(); make exceptions ...!
	// how to handle the protobuf msg properly
	//std::cout << "header[ ID:" << temp_.header_.id() << ", Size:" << temp_.header_.size() << " ]";
	//std::cout << "body[ content: " << temp_.body_.content() << " ]\n";
	//std::cout << "header[ ID:" << temp_.header_.id() << ", Size:" << temp_.header_.size() << " ]";
	//std::cout << "body[ content: " << temp_.body_.content() << " ]\n";
	
	read_deque_.push_back(read_msg_);

	ReadHeader();
}


}
