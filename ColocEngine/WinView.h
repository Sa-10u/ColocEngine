#pragma once
#include <Windows.h>
#include <cstdint>

#include<d3d12.h>
#include<dxgi1_4.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class WinView
{
public:

	WinView(uint32_t h, uint32_t w);
	~WinView();
	void Run();
	
private:

	static const uint32_t FrameAmmount = 2;

	HINSTANCE h_ins;
	HWND h_wnd;
	uint32_t h_;
	uint32_t w_;

	ID3D12Device* device_;
	IDXGISwapChain3* swpchain_;
	ID3D12Resource* colbuf_;
	ID3D12CommandQueue* cmdque_;
	ID3D12CommandAllocator* comaloc_;
	ID3D12CommandList* comlist_;
	ID3D12DescriptorHeap* heapRTV_;

	ID3D12Fence* fence_;
	HANDLE h_fence;
	uint64_t fencecnt_[FrameAmmount];
	uint64_t IND_frame;
	D3D12_CPU_DESCRIPTOR_HANDLE h_RTV;

private:

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	bool setup();
	bool initialize();
	void termination();
	void loop();

	bool initialize_D3D();
	void termination_D3D();
	void rendaring();
	void waitGPU();
	void present(uint32_t itv);
};

namespace WND_NAME
{
	const auto smp = TEXT("Sample");
}

