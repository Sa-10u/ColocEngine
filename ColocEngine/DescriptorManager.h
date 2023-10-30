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
	static bool Create(ID3D12Device* device_ ,)
};
