#pragma once
#include<cstdint>
#include<mutex>
#include<cassert>
#include<functional>

template<class T>
class PoolManager
{
public:

	PoolManager() :buffer_(nullptr), Active_(nullptr), Free_(nullptr), capacity_(NULL), count_(NULL) {};
	~PoolManager() { Term() };

	bool Init(uint32_t count)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		buffer_ = static_cast<uint8_t*>(new (sizeof(Item) * (count_ + 2));
		if (buffer_ == nullptr)	return false;
		capacity_ = count;
		for (auto i = 2u , j= 0u; i < capacity_ + 2; i++ , j++) {

			auto temp = GetItem(i);
			temp->ind_ = j;
		}

		Active_ = GetItem(0);

		return true;
	};
	void Term();
	
	T* Alloc(std::function<void(uint32_t, T*)>func = nullptr);
	void Free(T* addr);

private:

	struct Item
	{
		T val_;
		uint32_t ind_;
		Item* next;
		Item* prev;

		Item();
		~Item();
	};

private:
	uint32_t* buffer_;
	Item* Active_;
	Item* Free_;
	uint32_t capacity_;
	uint32_t count_;
	std::mutex mutex_;

private:
	Item* GetItem(uint32_t ind);
	Item* AssignItem(uint32_t ind);
};
