#pragma warning( disable : 4251 )
#include "../include/chat/chat.hpp"

int main()
{
	chat::message::Message msg;
	msg.body_.set_content("Hello?");
	msg.header_.set_id(UINT32_MAX);
	msg.header_.set_size(5);

	msg.body_.PrintDebugString();
	msg.header_.PrintDebugString();
	
	int d;
	std::cin >> d;
	std::cout << d << "\n";
	return 0;
}