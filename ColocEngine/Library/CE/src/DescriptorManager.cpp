#include "CE/DescriptorManager.h"

bool DescriptorHandle::HasCPU()
{
	return HCPU.ptr != NULL ? 1 : 0;
}

bool DescriptorHandle::HasGPU()
{
	return HGPU.ptr != NULL ? 1 : 0;
}

//------------------------------------------

bool DescriptorPool::Create(ID3D12Device* device_, const D3D12_DESCRIPTOR_HEAP_DESC* hp_desc, DescriptorPool** pool)
{
	if (!device_ || !hp_desc || !*pool)	return false;

	auto tempool = new(std::nothrow)DescriptorPool();
	if (!tempool)	return false;

	HRESULT res = device_->CreateDescriptorHeap
	(
		hp_desc,
		IID_PPV_ARGS(&(tempool->heap_))
	);
	if (FAILED(res)) { tempool->Release();return false; };
	if (!tempool->pool_.Init(hp_desc->NumDescriptors)) { tempool->Release(); return false; };

	tempool->size_ = device_->GetDescriptorHandleIncrementSize(hp_desc->Type);

	*pool = tempool;
	return true;
}

void DescriptorPool::AddRef()
{
	count_++;
}

void DescriptorPool::Release()
{
	count_--;

	if (count_ <= 0)
	{
		delete this;
	}
}

uint32_t DescriptorPool::GetCount()
{
	return count_;
}

uint32_t DescriptorPool::GetCountVH()
{
	return pool_.GetCountVacancy();
}

uint32_t DescriptorPool::GetCountUH()
{
	return pool_.GetCountUsed();
}

uint32_t DescriptorPool::GetCountH()
{
	return pool_.GetCapacity();
}

DescriptorHandle* DescriptorPool::AllocHandle()
{
	auto lamda = [&](uint32_t index,DescriptorHandle* h_des) 
		{
			auto h_CPU = heap_->GetCPUDescriptorHandleForHeapStart();
			auto h_GPU = heap_->GetGPUDescriptorHandleForHeapStart();

			h_CPU.ptr += size_ * index;
			h_GPU.ptr += size_ * index;

			h_des->HCPU = h_CPU;
			h_des->HGPU = h_GPU;
		};

	return (pool_.Alloc(lamda));
}

void DescriptorPool::FreeHandle(DescriptorHandle** handle)
{
	if (*handle == nullptr) return;

	pool_.Free(*handle);
	*handle = nullptr;
}

ID3D12DescriptorHeap* DescriptorPool::GetHeap()
{
	return heap_;
}

DescriptorPool::DescriptorPool():count_(1),pool_(),heap_(),size_(0)
{
}

DescriptorPool::~DescriptorPool()
{
	pool_.Term();
	heap_ = nullptr;
	size_ = 0;
}
