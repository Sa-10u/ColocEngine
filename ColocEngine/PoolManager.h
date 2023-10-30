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
	~PoolManager() { Term(); };
	PoolManager(const PoolManager&) = delete;
	operator = (const PoolManager&) = delete;

	bool Init(uint32_t count)
	{
		std::lock_guard<std::mutex> guard(mutex_);
		buffer_ = static_cast<uint8_t*>(new Item[count_ + 2]);
		if (buffer_ == nullptr)	return false;
		capacity_ = count;
		for (auto i = 2u , j= 0u; i < capacity_ + 2; i++ , j++) {

			auto temp = GetItem(i);
			temp->ind_ = j;
		}

		Active_ = GetItem(0);
		Active_->prev = Active_->next = Active_;
		Active_->ind_ = uint32_t(-1);

		Free_ = GetItem(1);
		Free_->ind_ = uint32_t(-2);

		for (auto i = 1u; i < capacity_ + 2; i++) {
			GetItem(i)->prev = nullptr;
			GetItem(i)->next = GetItem(i + 1);
		}

		GetItem(capacity_ + 1)->prev = Free_;
		count_ = 0;

		return true;
	};
	void Term()
	{
		std::lock_guard<std::mutex> guard(mutex_);

		if (buffer_ != nullptr)
		{
			delete buffer_;
			buffer_ = nullptr;
		}
		Active_ = nullptr;
		Free_ = nullptr;
		count_ = 0;
		capacity_ = 0;
	}
	
	T* Alloc(std::function<void(uint32_t, T*)>func = nullptr)
	{
		std::lock_guard<std::mutex> guard(mutex_);

		if (Active_->next == Free_ || count_ + 1 > capacity_)
		{
			return nullptr;
		}

		auto temp = Free_->next;
		Free_->next = temp->next;

		temp->prev = Active_->prev;
		temp->next = Active_;
		temp->prev->next = temp->next->prev = temp;

		count_++;]

		auto val = new ((void*)Item) T();

		if (func != nullptr)
		{
			func(temp->ind_, val);
		}

		return val;
	}

	void Free(T* addr)
	{
		if (addr == nullptr)	return;

		std::lock_guard<std::mutex> guard(mutex_);

		auto temp = reinterpret_cast<Item*>(val);
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;

		temp->prev = nullptr;
		temp->next = Free_->next;

		Free_->next = temp;
		count_--;
	}

	uint32_t GetCountUsed()
	{
		return count_;
	}

	uint32_t GetCountVacancy()
	{
		return capacity_ - count_;
	}

	uint32_t GetCapacity()
	{
		return capacity_;
	}
private:

	struct Item
	{
		T val_;
		uint32_t ind_;
		Item* next;
		Item* prev;

		Item() :val(), ind_(NULL), next(nullptr), prev(nullptr) {};
		~Item() {};
	};

private:
	uint32_t* buffer_;
	Item* Active_;
	Item* Free_;
	uint32_t capacity_;
	uint32_t count_;
	std::mutex mutex_;

private:
	Item* GetItem(uint32_t ind)
	{
		return reinterpret_cast<Item*>(buffer_ + (ind) * sizeof(Item));
	};

	Item* AssignItem(uint32_t ind)
	{
		auto temp = (buffer_ + (ind) * sizeof(Item));
		return new(buf) Item;
	};
};
