#pragma once
#include "predef.h"
#include "server_connection.h"
#include "boost/functional/hash.hpp"

namespace chat {
// Created when client connected to the Server 
// this instance will be handled by Server stored in the map
struct ConnectionInfo
{
	using clock = std::chrono::system_clock;
	using ConSharedPtr = std::shared_ptr<ServerConnection>;

	uint32_t id;
	clock::time_point connected_time;
	ConSharedPtr ptr;

	ConnectionInfo()
		: id(0), connected_time(clock::now()), ptr(nullptr) {}

	ConnectionInfo(uint32_t _id, ConSharedPtr _ptr)
		: id(_id), connected_time(clock::now()), ptr(_ptr) {}

	// not used
	bool operator==(const ConnectionInfo& other) const
	{
		return id == other.id && connected_time == other.connected_time;
	}
};

struct KeyHasher
{
	// it can make system slow, Test required
	std::size_t operator()(const ConnectionInfo& key) const
	{
		using std::size_t;
		//using std::hash;
		//using std::string;
		using namespace std::chrono;

		// std::chrono::time_point converting discuss in stackoverflow
		// time_point(milliseconds) -> epoch -> duration_cast(long type)
		// https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
		auto epoch =
			time_point_cast<milliseconds>(key.connected_time).time_since_epoch();
		long time_key = duration_cast<milliseconds>(epoch).count();

		using boost::hash_value;
		using boost::hash_combine;

		size_t seed = 0;
		hash_combine(seed, hash_value(key.id));
		hash_combine(seed, hash_value(time_key));
		
		return seed;
	}
};
}