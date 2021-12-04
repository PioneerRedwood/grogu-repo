#include "ServerConnection.h"

namespace chat{

ServerConnection::ServerConnection(
	boost::asio::io_context& context, TSDeque<Message>& read_deque, boost::asio::ip::tcp::socket socket)
	: context_(context), read_strand_(context), write_strand_(context),
	socket_(std::move(socket)), read_deque_(read_deque)
{}

void ServerConnection::Start()
{
	Read();
}

void ServerConnection::Send(const Message& msg)
{
	boost::asio::post(context_, [&, msg]()->void
		{
			//CreateMessage(temp_, SERVER_ID,
			//	"Server Update to #" + std::to_string(client.second.id) + " Client");

			bool isWriting = !write_deque_.empty();
			write_deque_.push_back(msg);
			if (!isWriting)
			{
				Write();
			}
		});
}

void ServerConnection::Send(const uint32_t _id, const std::string& _content)
{

}

void ServerConnection::Read()
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
										// Get non-owning poiner exception error!
										// Don't use
										//PrintMessage(&temp_);
										
										// if it  is important to keep on then Store() ..!
										Store();
									}
								}));
					}
				}
			}));
}

void ServerConnection::Write()
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
										cout << "Write completed .. \t"
											<< "header bytes: " << header_bytes << "\t"
											<< "body bytes: " << body_bytes << "\n";
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

void ServerConnection::Store()
{
	//temp_.body_.PrintDebugString();
	//temp_.header_.PrintDebugString();

	std::cout << "header[ ID:" << temp_.header_.id() << ", Size:" << temp_.header_.size() << " ]";
	std::cout << "body[ content: " << temp_.body_.content() << " ]\n";

	read_deque_.push_back(temp_);

	Read();
}


}
