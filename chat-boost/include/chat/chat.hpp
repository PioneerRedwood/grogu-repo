#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif 

#pragma warning( disable : 4251 )

#include "ChattingMessage.pb.h"
#include "message_wrapper.hpp"
#include "tsdeque.hpp"
#include "tcp_connection.hpp"

