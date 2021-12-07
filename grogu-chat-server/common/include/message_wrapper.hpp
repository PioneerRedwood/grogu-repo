#pragma once
#include <iostream>
#include <string>

#include "chatting_message.pb.h"
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/text_format.h"

namespace chat {
namespace message {

// All the message types	
using header = grogu::chat::SimpleHeader;
using body = grogu::chat::SimpleMessage;

struct Message
{
	header header_;
	body body_;

	//Message() {}
};

// Set data into the src properly with NO allocation!
const static void SetMessage(Message* const msg, const uint32_t _id, const std::string& _content)
{
	msg->header_.set_id(_id);
	msg->header_.set_size(static_cast<uint32_t>(_content.size()));
	msg->body_.set_content(_content);
}

const static void PrintMessage(const Message* msg)
{
	// Serialize and print into console
	msg->header_.PrintDebugString();
	msg->body_.PrintDebugString();
}

template<class Type>
class MessageWrapper
{
	//using msg = std::shared_ptr<Type>;
public:
	MessageWrapper(std::shared_ptr<Type> src = std::shared_ptr<Type>())
		: msg_(src)
	{}
	
	void SetData(std::shared_ptr<Type> msg)
	{
		msg_ = msg;
	}

	std::shared_ptr<Type> GetData()
	{
		return msg_;
	}

	// Pack message to given buffer
	// if failed, return false, otherwise true
	bool Pack(std::vector<uint8_t>& buffer) const
	{
		if (!msg_)
		{
			return false;
		}

		unsigned size = msg_->ByteSize();
		//buffer.resize()
	}

	// Unpack message to given 
	bool Unpack(const std::vector<uint8_t>& buffer)
	{

	}

private:
	// encode

	// decode

	std::shared_ptr<Type> msg_;

};
} // message
} // chat