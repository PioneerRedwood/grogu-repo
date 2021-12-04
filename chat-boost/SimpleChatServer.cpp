#include "SimpleChatServer.h"

namespace chat {
SimpleChatServer::SimpleChatServer(
	boost::asio::io_context& context, const int port, const uint16_t period)
	: context_(context), 
	acceptor_{ context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)},
	accept_strand_(context), update_timer_(context, std::chrono::seconds(period)), update_period_(period)
{

}

void SimpleChatServer::Start()
{
	std::cout << "SimpleChatServer started ..\n";

	Accept();
	TimerUpdate();

	thr_ = thread([&]()->void { context_.run(); });
}

void SimpleChatServer::Tick(size_t max_msg, bool is_wait)
{
	/*size_t count = 0;
	while ((count < max_msg_) && !read_deque_.empty())
	{
		auto temp = read_deque_.pop_front();

		OnMessage(temp);

		count++;
	}*/
}

void SimpleChatServer::Accept()
{
	// TODO: Accept new connection from the client
	acceptor_.async_accept(
		[this](auto& error, boost::asio::ip::tcp::socket socket)
		{
			// Handle if error occured
			if (error)
			{
				cerr << "Error: SimpleChatServer accept.. " << error << "\n";
				return;
			}

			// connected client info print
			std::stringstream ss;
			ss << socket.remote_endpoint();
			cout << "Connected " << ss.str() << "\n";

			// make shared_ptr of client
			conn_ptr conn =
				std::make_shared<ServerConnection>(
					context_, read_deque_, std::move(socket));
			conn->Start();

			curr_id_++;
			clients_.emplace(curr_id_, ConnectionInfo(curr_id_, std::move(conn)));

			/*
			SimpleMessage msg;
			msg.set_content("Hello client");
			msg.set_owner_id(SERVER_ID);
			msg.set_result(curr_id_);

			ShowPrettyMessage(msg);

			clients_[curr_id_].ptr->Send(std::move(msg));
			*/
			//SetMessage()

			Accept();

		});
}

void SimpleChatServer::TimerUpdate()
{
	update_timer_.expires_at(update_timer_.expiry() + std::chrono::seconds(update_period_));
	update_timer_.async_wait(
		[this](auto& error)->void
		{
			if (error)
			{
				return;
			}

			cout << "Server Update ..\n";

			if (!clients_.empty())
			{
				for (const auto& client : clients_)
				{
					//CreateMessage(temp_, SERVER_ID, 
					//	"Server Update to #" + std::to_string(client.second.id) + " Client");

					//client.second.ptr->Send();
				}
			}

			TimerUpdate();
		});
}

void SimpleChatServer::OnConnect(conn_ptr client, uint32_t id)
{
	cout << "OnConnect ..\n";
}

void SimpleChatServer::OnDisconnect(conn_ptr client)
{
	cout << "OnDisConnect ..\n";
}

void SimpleChatServer::OnMessage(const Message& msg)
{
	
}

}
