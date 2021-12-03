#pragma once
#include <iostream>
#include <string>
#include "ChattingMessage.pb.h"
#include "google/protobuf/text_format.h"

namespace chat {

	using SimpleMessage = shelby::chat::proto3::SimpleChattingMessage;
const static void ShowPrettyMessage(const SimpleMessage& msg)
{
	// Serialize and print into console
	if (std::string str; google::protobuf::TextFormat::PrintToString(msg, &str))
	{
		std::cout << str << "\n";
	}
}


}