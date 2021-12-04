#include "ClientConnection.h"
#include "MessageWrapper.h"

namespace chat {

ClientConnection::ClientConnection(boost::asio::io_context& context)
	: context_(context), socket_(context), 
	read_strand_(context), write_strand_(context),
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
	//std::cout << "Begin to read.. " << read_deque_.size() << "\n";
	boost::asio::async_read(socket_,
		boost::asio::buffer(&temp_.header_, sizeof(header)),
		read_strand_.wrap([&](auto& read_header_error, size_t header_bytes)->void
			{
				// Handle error
				if (read_header_error)
				{
					cerr << "Error: read " << read_header_error << "\n";
					return;
				}
				else
				{
					if (temp_.header_.size() > 0)
					{
						boost::asio::async_read(socket_,
							boost::asio::buffer(&temp_.body_, size_t(temp_.header_.size())),
							read_strand_.wrap([&](auto& read_body_error, size_t body_bytes)->void
								{
									if (read_body_error)
									{
										cerr << "Error: read " << read_body_error << "\n";
										return;
									}
									else
									{
										//PrintMessage(&temp_);

										// if it  is important to keep on then Store() ..!
										Store();
									}
								}));
					}
				}
			}));
}

void ClientConnection::Write()
{
	//std::cout << "Begin to write .." << write_deque_.size() << "\n";
	boost::asio::async_write(socket_,
		boost::asio::buffer(&write_deque_.front().header_, sizeof(header)),
		write_strand_.wrap([&](auto& write_header_error, size_t header_bytes)->void
			{
				// Handle error
				if (write_header_error)
				{
					cerr << "Error: write " << write_header_error << "\n";
					return;
				}
				else
				{
					if (write_deque_.front().header_.size() > 0)
					{
						boost::asio::async_write(socket_,
							boost::asio::buffer(&write_deque_.front().body_, size_t(write_deque_.front().header_.size())),
							write_strand_.wrap([&](auto& write_body_error, size_t body_bytes)->void
								{
									if (write_body_error)
									{
										cerr << "Error: write " << write_body_error << "\n";
										return;
									}
									else
									{
										// if completed with success print it
										cout << "Write completed[ "
											<< "header bytes: " << header_bytes << "\t"
											<< "body bytes: " << body_bytes << " ]\n";
									}
								}));
					}

					// if there are more data to send? Call Write again
					if (!write_deque_.empty())
					{
						write_deque_.pop_front();
					}
				}
			}));
}

void ClientConnection::Store()
{
	std::cout << "header[ ID:" << temp_.header_.id() << ", Size:" << temp_.header_.size() << " ]";
	std::cout << "body[ content: " << temp_.body_.content() << " ]\n";

	read_deque_.push_back(temp_);

	Read();
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

void ClientConnection::Send(const Message& msg)
{
	boost::asio::post(context_, [&, msg]()->void
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
	ping_timer_.expires_at(ping_timer_.expiry() + std::chrono::seconds(1));
	ping_timer_.async_wait([&](auto& error)->void
		{
			if (error)
			{
				return;
			}

			Message msg;
			// the string value not set on msg ...
			msg.body_.set_content(PING);
			msg.header_.set_id(1219);
			msg.header_.set_size(5);

			PrintMessage(&msg);

			Send(std::move(msg));
			Ping();
		});

}
}