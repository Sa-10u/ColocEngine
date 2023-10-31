#include "DescriptorManager.h"

bool DescriptorHandle::HasCPU()
{
	return HCPU.ptr != NULL ? 1 : 0;
}

bool DescriptorHandle::HasGPU()
{
	return HGPU.ptr != NULL ? 1 : 0;
}



bool DescriptorPool::Create(ID3D12Device* device_, const D3D12_DESCRIPTOR_HEAP_DESC* hp_desc, DescriptorPool** pool)
{
	return false;
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
	return 0;
}

uint32_t DescriptorPool::GetCountUH()
{
	return 0;
}

uint32_t DescriptorPool::GetCountH()
{
	return 0;
}

DescriptorHandle DescriptorPool::AllocHandle()
{
	return DescriptorHandle();
}

void DescriptorPool::FreeHandle(DescriptorHandle** handle)
{
}

ID3D12DescriptorHeap* DescriptorPool::GetHeap()
{
	return nullptr;
}

DescriptorPool::DescriptorPool():count_(1),pool_(),heap_(),size_(0)
{
}

DescriptorPool::~DescriptorPool()
{
	pool_.Term();
	heap_->Release();
	size_ = 0;
}
