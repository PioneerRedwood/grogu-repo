#include <iostream>
#include <climits>

#include <boost/asio.hpp>
#include "ChattingMessage.pb.h"

int proto_test_main()
{
	shelby::chat::proto3::SimpleChattingMessage msg;
	
	msg.set_content("This is for you, L.");
	msg.set_owner_id(INT32_MAX);
	msg.set_result(INT32_MIN);

	std::string result = "";
	if (msg.SerializeToString(&result))
	{
		//std::cout << result << std::endl;
		std::cout << "msg::content: " << msg.content() << "\n";
		std::cout << "msg::owner_id: " << msg.owner_id() << "\n";
		std::cout << "msg::result: " << msg.result() << "\n";
	}
	else
	{
		std::cout << "failed to serialize the message\n";
	}

	return 0;
}