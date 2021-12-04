// new version thread-safe STL wrapper
// required complier version C++17 or later, mutex scoped_lock
// 2021-12-03 header-only

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

	/*
	const T& front();

	const T& back();

	T pop_front();

	T pop_back();

	void push_back(const T& item);

	bool empty();

	size_t size();

	void clear();

	void wait();

	*/

	const T& front()
	{
		std::scoped_lock lock(mutex_);
		return deque_.front();
	}

	const T& back()
	{
		std::scoped_lock lock(mutex_);
		return deque_.back();
	}

	T pop_front()
	{
		std::scoped_lock(mutex_);
		auto t = move(deque_.front());
		deque_.pop_front();
		return t;
	}

	T pop_back()
	{
		std::scoped_lock(mutex_);
		auto t = std::move(deque_.back());
		deque_.pop_back();
		return t;
	}

	void push_back(const T& item)
	{
		std::scoped_lock lock(mutex_);
		deque_.emplace_back(move(item));

		std::unique_lock<mutex> ul(mutex_block_);
		condition_.notify_one();
	}

	bool empty()
	{
		std::scoped_lock lock(mutex_);
		return deque_.empty();
	}

	size_t size()
	{
		std::scoped_lock lock(mutex_);
		return deque_.size();
	}

	void clear()
	{
		std::scoped_lock lock(mutex_);
		deque_.clear();
	}

	void wait()
	{
		while (empty())
		{
			std::unique_lock<mutex> ul(mutex_block_);
			condition_.wait(ul);
		}
	}
};
}