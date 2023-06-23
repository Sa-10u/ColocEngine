#pragma once
#include <cstdint>
#include <Windows.h>

#include<DirectXMath.h>
#include<d3d12.h>
#include<dxgi1_4.h>
#include"MACRO.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace DirectX;

//------------

class D3d
{
public:

	static D3d* Create();
	static void Kill();

	bool Initialize(HWND hwnd , uint32_t h , uint32_t w);
	void Termination();
	void Run(int interval);

private:
	D3d();
	~D3d();

	void write();
	void waitGPU();
	void present(uint32_t itv);

private:
	static const uint32_t FrameAmmount = 2;

	ID3D12Device* device_;
	IDXGISwapChain3* swpchain_;
	ID3D12Resource* colbuf_[FrameAmmount];
	ID3D12CommandQueue* cmdque_;
	ID3D12CommandAllocator* cmdalloc_[FrameAmmount];
	ID3D12GraphicsCommandList* cmdlist_;
	ID3D12DescriptorHeap* heapRTV_;

	ID3D12Fence* fence_;
	HANDLE event_fence;
	uint64_t fencecnt_[FrameAmmount];
	uint64_t IND_frame;
	D3D12_CPU_DESCRIPTOR_HANDLE h_RTV[FrameAmmount];


	//----

	static D3d* instance;
};

extern D3d* D3D;		//<---- use this.