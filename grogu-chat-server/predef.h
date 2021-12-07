#pragma once

// at least WINDOWS 7
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif 

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <thread>

// boost-asio
/*
io_context, strand, tcp, udp, steady_timer

*/
#include "boost/asio/io_context.hpp"
#include "boost/asio/io_context_strand.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/ip/udp.hpp"
#include "boost/asio/steady_timer.hpp"

// google-protobuf
// about warning
#pragma warning( disable : 4251 )

#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/text_format.h"

// custom common header

// custom define!
