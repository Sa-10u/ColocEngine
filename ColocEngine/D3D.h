#pragma once
#include <cstdint>
#include <Windows.h>

#include<DirectXMath.h>
#include<d3d12.h>
#include<dxgi1_4.h>
#include"MACRO.h"
#include"BUFFER.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

//------------

class D3d
{
public:

	D3d();
	~D3d();

	bool Initialize(HWND hwnd , uint32_t h , uint32_t w);
	bool InitGBO();
	bool InitPSO();
	void Termination();
	void TermGBO();		//Graphic Buffer Object
	void Run(int interval);

	void SetHeight(float h);
	void SetWidth(float w);
	float GetHeight();
	float GetWidth();

private:

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

	D3D12_RESOURCE_BARRIER brr;

	ID3D12DescriptorHeap* heapCBV_;
	ID3D12RootSignature* rootsig_;
	ID3D12PipelineState* PSO;

	ID3D12Resource* VB;
	ID3D12Resource* CB[FrameAmmount];
	D3D12_VERTEX_BUFFER_VIEW VBV;
	CBUFFERVIEW<WVP> CBV[FrameAmmount];

	D3D12_VIEWPORT view_;
	D3D12_RECT rect_;

	float angle_;

	//-----

	float Height;
	float Width;

	//-------

};

