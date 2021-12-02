// new version thread-safe STL wrapper
// required complier version C++17 or later, mutex scoped_lock

#pragma once
#include <mutex>
#include <deque>
#include <condition_variable>

using namespace std;

namespace chat {
template<typename T>
class TSDeque
{
private:
	std::mutex mutex_ = {};
	std::deque<T> deque_ = {};
	std::condition_variable condition_ = {};
	std::mutex mutex_block_ = {};

public:
	TSDeque() = default;
	TSDeque(const TSDeque<T>&) = delete;
	virtual ~TSDeque() { clear(); }

	const T& front();

	const T& back();

	T pop_front();

	T pop_back();

	void push_back(const T& item);

	bool empty();

	size_t size();

	void clear();

	void wait();
};
}