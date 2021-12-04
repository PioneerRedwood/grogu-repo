#pragma once
#include <iostream>
#include <string>

#include "ChattingMessage.pb.h"
#include "google/protobuf/text_format.h"

namespace chat {

// All the message types	
using header = shelby::chat::proto3::SimpleHeader;
using body = shelby::chat::proto3::SimpleMessage;

struct Message
{
	header header_;
	body body_;
};

// Set data into the src properly with NO allocation!
const static void SetMessage(Message* const msg, const uint32_t _id, const std::string& _content)
{
	msg->header_.set_id(_id);
	msg->header_.set_size(_content.size());
	msg->body_.set_content(_content);
}

const static void PrintMessage(const Message* msg)
{
	// Serialize and print into console
	msg->header_.PrintDebugString();
	msg->body_.PrintDebugString();
}

}