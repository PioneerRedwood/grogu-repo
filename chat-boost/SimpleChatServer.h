#pragma once
#include <unordered_map>
#include <thread>

#include "boost/asio.hpp"
#include "ServerConnection.h"
#include "ConnectionInfo.h"
#include "TSDeque.hpp"
#include "MessageWrapper.h"

namespace chat {
class SimpleChatServer
{
	using conn_ptr = std::shared_ptr<ServerConnection>;
public:
	explicit SimpleChatServer(
		boost::asio::io_context& context,
		const int port,
		const uint16_t period);

	void Start();
	void Tick(size_t max_msg = -1, bool is_wait = false);

private:
	void Accept();
	void TimerUpdate();

	virtual void OnConnect(conn_ptr client, uint32_t id);
	virtual void OnDisconnect(conn_ptr client);
	virtual void OnMessage(const Message& msg);

private:
	boost::asio::io_context& context_;
	boost::asio::io_context::strand accept_strand_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::endpoint endpoint_;

	Message temp_;
	TSDeque<Message> read_deque_;
	// custom key map does not work well .. :p
	//std::unordered_map<ConnectionInfo, conn_ptr, KeyHasher> clients_;
	std::unordered_map<uint32_t, ConnectionInfo> clients_;

	boost::asio::steady_timer update_timer_;
	uint16_t update_period_ = 0;

	uint32_t max_msg_ = UINT32_MAX >> 8;
	uint32_t curr_id_ = 0;
	
	const uint32_t SERVER_ID = 0;
	std::thread thr_;
};
}