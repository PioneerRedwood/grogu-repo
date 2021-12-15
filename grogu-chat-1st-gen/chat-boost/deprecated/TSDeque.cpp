//#include "TSDeque.h"
//
//namespace chat {
//
//template<typename T>
//const T& TSDeque<T>::front()
//{
//	std::scoped_lock lock(mutex_);
//	return deque_.front();
//}
//
//template<typename T>
//const T& TSDeque<T>::back()
//{
//	std::scoped_lock lock(mutex_);
//	return deque_.back();
//}
//
//template<typename T>
//T TSDeque<T>::pop_front()
//{
//	std::scoped_lock(mutex_);
//	auto t = move(deque_.front());
//	deque_.pop_front();
//	return t;
//}
//
//template<typename T>
//T TSDeque<T>::pop_back()
//{
//	std::scoped_lock(mutex_);
//	auto t = std::move(deque_.back());
//	deque_.pop_back();
//	return t;
//}
//
//template<typename T>
//void TSDeque<T>::push_back(const T& item)
//{
//	std::scoped_lock lock(mutex_);
//	deque_.emplace_back(move(item));
//
//	std::unique_lock<mutex> ul(mutex_block_);
//	condition_.notify_one();
//}
//
//template<typename T>
//bool TSDeque<T>::empty()
//{
//	std::scoped_lock lock(mutex_);
//	return deque_.empty();
//}
//
//template<typename T>
//size_t TSDeque<T>::size()
//{
//	std::scoped_lock lock(mutex_);
//	return deque_.size();
//}
//
//template<typename T>
//void TSDeque<T>::clear()
//{
//	std::scoped_lock lock(mutex_);
//	deque_.clear();
//}
//
//template<typename T>
//void TSDeque<T>::wait()
//{
//	while(empty())
//	{
//		std::unique_lock<mutex> ul(mutex_block_);
//		condition_.wait(ul);
//	}
//}
//}