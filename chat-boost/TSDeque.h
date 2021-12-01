// new version thread-safe STL wrapper
// required complier version C++17 or later, mutex scoped_lock
#pragma once

// mutex vs semaphore
#include <mutex>
//#include <semaphore> // required C++20 or later
#include <deque>
#include <condition_variable>

using namespace std;

template<typename T>
class TSDeque
{
private:
	mutex mutex_ = {};
	deque<T> deque_ = {};
	condition_variable condition_ = {};
	mutex mutex_block_ = {};

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