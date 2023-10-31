#pragma once
#include<d3d12.h>
#include<atomic>
#include"PoolManager.h"

class DescriptorHandle
{
public:
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;

	bool HasCPU();
	bool HasGPU();
};

class DescriptorPool
{
public:
	static bool Create(ID3D12Device* device_, const D3D12_DESCRIPTOR_HEAP_DESC* hp_desc, DescriptorPool** pool);
	void AddRef();
	void Release();
	uint32_t GetCount();
	uint32_t GetCountVH();
	uint32_t GetCountUH();
	uint32_t GetCountH();
	DescriptorHandle AllocHandle();
	void FreeHandle(DescriptorHandle** handle);
	ID3D12DescriptorHeap* GetHeap();

private:
	std::atomic<uint32_t> count_;
	PoolManager<DescriptorHandle>  pool_;
	ID3D12DescriptorHeap* heap_;
	uint32_t size_;

private:
	DescriptorPool();
	~DescriptorPool();
	DescriptorPool(const DescriptorPool&) = delete;
	void operator = (const DescriptorPool&) = delete;
};
