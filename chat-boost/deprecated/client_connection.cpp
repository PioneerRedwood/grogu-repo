#include "client_connection.h"
#include "message_wrapper.h"

namespace chat {

ClientConnection::ClientConnection(boost::asio::io_context& context)
	: context_(context), socket_(context), 
	read_strand_(context), write_strand_(context),
	ping_timer_(context, chrono::seconds(1)),
	retry_timer_(context, chrono::seconds(1))
{
	//temp_ = std::make_shared<Message>();
}

void ClientConnection::Start(const std::string& ip, const int port)
{
	std::cout << "Client Started\n";
	std::cout << "TryConnect .. \n";
	Connect("127.0.0.1", 12190);

	if (!socket_.is_open())
	{
		TryConnect();
	}
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
				TryConnect();
				return;
			}

			// TODO: notify to all about "gracefully" connected to server
			std::cout << "Connected gracefully ..\n";

			Ping();
			ReadHeader();
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

// copy 
void ClientConnection::Send(const Message& msg)
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
void ClientConnection::ReadHeader()
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

void ClientConnection::ReadBody(std::size_t size)
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

void ClientConnection::WriteHeader()
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

void ClientConnection::WriteBody(std::size_t size)
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

void ClientConnection::Store()
{
	// if use temp_.PrintDebugString(); make exceptions ...!
	// how to handle the protobuf msg properly
	//std::cout << "header[ ID:" << temp_.header_.id() << ", Size:" << temp_.header_.size() << " ]";
	//std::cout << "body[ content: " << temp_.body_.content() << " ]\n";
	
	read_deque_.push_back(read_msg_);

	ReadHeader();
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
			
			write_msg_.body_.set_content("CONTENTS");
			write_msg_.header_.set_id(1219);
			write_msg_.header_.set_size(write_msg_.body_.ByteSizeLong());

			Send(write_msg_);
			Ping();
		});

}
}