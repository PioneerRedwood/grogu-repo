#pragma once
#include <boost/asio.hpp>
#include <memory>

namespace chat{
class TcpConnection
	: std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(boost::asio::io_context& context)
		: context_(context), socket_(context)
	{

	}

protected:
	void Read()
	{
		// TODO: 소켓으로부터 데이터 읽어오기
		//boost::asio::async_read(socket_, boost::asio::buffer(), 
	}

	void Write()
	{
		// TODO: 소켓에 데이터 쓰기

	}

protected:
	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	
};

// TODO: Depends on how many handle the messages received from the server
// this class must handle only a communication for connection
class ClientConnection : public TcpConnection
{
public:
	// TODO: required connection ip, port
	ClientConnection(boost::asio::io_context& context, const std::string& ip, const int port)
		: TcpConnection(context), ip_(ip), port_(port)
	{

	}

	void Start()
	{
		// Start async_connect to server
		socket_.async_connect(
			boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip_), port_),
			[&](std::error_code& error)->void
			{
				if (error) return;

				// TODO: notify to all about "gracefully" connected to server
				printf("connected server\n");

				Read();
			}
		);
	}

	// TODO: Send data to server
	template<typename T>
	int Send(T data)
	{
		// template designed function,
		Write();
	}

private:
	const std::string& ip_;
	int port_;
};

// TODO: In server case, I/O handled by multi-threading.
// Hardly recommended, USE -- boost::asio::io_context::strand -- 
class ServerConnection : public TcpConnection
{
public:
	// TODO: required opening port
	ServerConnection(boost::asio::io_context& context, const int port)
		: TcpConnection(context)
	{

	}

	void Start()
	{

	}
};
}