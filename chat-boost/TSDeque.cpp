#include "TSDeque.h"

using namespace std;

template<typename T>
inline const T& TSDeque<T>::front()
{
	scoped_lock lock(mutex_);
	return deque_.front();
}

template<typename T>
const T& TSDeque<T>::back()
{
	scoped_lock lock(mutex_);
	return deque_.back();
}

template<typename T>
T TSDeque<T>::pop_front()
{
	scoped_lock(mutex_);
	auto t = move(deque_.front());
	deque_.pop_front();
	return t;
}

template<typename T>
T TSDeque<T>::pop_back()
{
	scoped_lock(mutex_);
	auto t = move(deque_.back());
	deque_.pop_back();
	return t;
}

template<typename T>
void TSDeque<T>::push_back(const T& item)
{
	scoped_lock lock(mutex_);
	deque_.emplace_back(move(item));

	unique_lock<mutex> ul(mutex_block_);
	condition_.notify_one();
}

template<typename T>
bool TSDeque<T>::empty()
{
	scoped_lock lock(mutex_);
	return deque_.empty();
}

template<typename T>
size_t TSDeque<T>::size()
{
	scoped_lock lock(mutex_);
	return deque_.size();
}

template<typename T>
void TSDeque<T>::clear()
{
	scoped_lock lock(mutex_);
	deque_.clear();
}

template<typename T>
void TSDeque<T>::wait()
{
	while(empty())
	{
		unique_lock<mutex> ul(mutex_block_);
		condition_.wait(ul);
	}
}