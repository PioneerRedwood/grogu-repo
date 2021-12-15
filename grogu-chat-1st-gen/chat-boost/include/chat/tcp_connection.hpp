#pragma once
#include "boost/asio.hpp"
#include "tsdeque.hpp"
#include "message_wrapper.hpp"

namespace chat{

class TcpConnection
	: public std::enable_shared_from_this<TcpConnection>
{
protected:
	// for server
	explicit TcpConnection(boost::asio::io_context& context,
		boost::asio::ip::tcp::socket socket, TSDeque<chat::message::Message>& read_deque)
		: context_(context), socket_(std::move(socket)), read_deque_(read_deque),
		read_strand_(context), write_strand_(context)
	{

	}

	// for client
	explicit TcpConnection(boost::asio::io_context& context, TSDeque<chat::message::Message>& read_deque)
		:context_(context), socket_(context), read_deque_(read_deque),
		read_strand_(context), write_strand_(context)
	{

	}

protected:
	void ReadHeader()
	{
		boost::asio::async_read(socket_,
			//boost::asio::streambuf(),
			boost::asio::buffer(&read_msg_.header_, sizeof(chat::message::header)),
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
				else
				{
					ReadHeader();
				}
			}
		}));
	}

	void ReadBody(std::size_t size)
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

	void WriteHeader()
	{
		boost::asio::async_write(socket_,
			boost::asio::buffer(&write_deque_.front().header_, sizeof(chat::message::header)),
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

	void WriteBody(std::size_t size)
	{
		boost::asio::async_write(socket_,
			boost::asio::buffer(&write_deque_.front().body_, size),
			write_strand_.wrap([self = shared_from_this(), header_size = size](auto& error, size_t bytes)->void
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

	void Store()
	{
		read_deque_.push_back(read_msg_);

		ReadHeader();
	}

protected:	
	virtual void Send(const chat::message::Message& msg)
	{

	}

	const boost::asio::ip::tcp::socket& GetSocket() { return socket_; }

protected:
	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	
	boost::asio::io_context::strand read_strand_;
	boost::asio::io_context::strand write_strand_;

	chat::message::Message read_msg_;

	TSDeque<chat::message::Message>& read_deque_;
	TSDeque<chat::message::Message> write_deque_;
};
}