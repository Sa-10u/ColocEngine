#include "D3D.h"
#include<cassert>

#define __guidof __uuidof

D3d* D3D = D3d::Create();

D3d* D3d::instance = nullptr;

D3d* D3d::Create()
{
	if (instance == nullptr)
	{
		instance = new D3d;
	}
	
	return instance;
}

void D3d::Kill()
{
    delete instance;
    instance = nullptr;
}

bool D3d::Initialize(HWND hwnd, uint32_t h, uint32_t w)
{
    bool FAIL = 0;

    auto res = D3D12CreateDevice
    (
        nullptr,
        D3D_FEATURE_LEVEL_12_1,
        __guidof(device_),
        reinterpret_cast<void**>(&device_)
    );
    if (FAILED(res))               return FAIL;


    D3D12_COMMAND_QUEUE_DESC cmddesc = {};

    {
        cmddesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        cmddesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        cmddesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        cmddesc.NodeMask = NULL;
    }
    res = device_->CreateCommandQueue(&cmddesc, __guidof(cmdque_), reinterpret_cast<void**>(&cmdque_));
    if (FAILED(res))     return FAIL;


    IDXGIFactory4* fact = nullptr;
    res = CreateDXGIFactory2(0,__guidof(fact), reinterpret_cast<void**>(&fact));
    if (FAILED(res))     return 0;


    DXGI_SWAP_CHAIN_DESC desc = {};
    {
        desc.BufferDesc.Height = h;
        desc.BufferDesc.Width = w;
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = FrameAmmount;
        desc.OutputWindow = hwnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    }
    IDXGISwapChain* p_swch = nullptr;
    res = fact->CreateSwapChain(cmdque_, &desc, &p_swch);   //------failed
    if (FAILED(res))
    {
        SAFE_RELEASE(fact);
        return FAIL;
    }

    res = p_swch->QueryInterface(__guidof(swpchain_), reinterpret_cast<void**>(&swpchain_));
    if (FAILED(res))
    {
        SAFE_RELEASE(fact);
        SAFE_RELEASE(p_swch);
        return FAIL;
    }

    IND_frame = swpchain_->GetCurrentBackBufferIndex();
    SAFE_RELEASE(fact);
    SAFE_RELEASE(p_swch);


    for (int i = 0u; i < FrameAmmount; i++) {

        res = device_->CreateCommandAllocator
        (
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            __guidof(cmdalloc_[i]),
            reinterpret_cast<void**>(&cmdalloc_[i])
        );

        if (FAILED(res)) return FAIL;
    }

    res = device_->CreateCommandList
    (
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmdalloc_[IND_frame],
        nullptr,
        __guidof(cmdlist_),
        reinterpret_cast<void**>(&cmdlist_)
    );
    if (FAILED(res)) return FAIL;

    D3D12_DESCRIPTOR_HEAP_DESC hpdesc;

    {
        hpdesc.NumDescriptors = FrameAmmount;
        hpdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        hpdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        hpdesc.NodeMask = 0;
    }

    res = device_->CreateDescriptorHeap(&hpdesc, __guidof(heapRTV_), reinterpret_cast<void**>(&heapRTV_));

    if (FAILED(res))    return FAIL;


    D3D12_CPU_DESCRIPTOR_HANDLE handle = heapRTV_->GetCPUDescriptorHandleForHeapStart();
    UINT incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    for (UINT i = 0u; i < FrameAmmount; i++) {

        res = swpchain_->GetBuffer(i, IID_PPV_ARGS(&colbuf_[i]));
        if (FAILED(res))     return FAIL;

        D3D12_RENDER_TARGET_VIEW_DESC rtvdesc = {};

        rtvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtvdesc.Texture2D.MipSlice = NULL;
        rtvdesc.Texture2D.PlaneSlice = NULL;

        device_->CreateRenderTargetView(colbuf_[i], &rtvdesc, handle);

        h_RTV[i] = handle;
        handle.ptr += incre;
    }

    res = device_->CreateFence
    (
        fencecnt_[IND_frame],
        D3D12_FENCE_FLAG_NONE,
        __guidof(fence_),
        reinterpret_cast<void**>(&fence_)
    );

    if (FAILED(res))     return FAIL;

    event_fence = CreateEvent(nullptr, false, false, nullptr);
    if (event_fence == nullptr)  return FAIL;

    return true;
}

void D3d::Termination()
{
    waitGPU();
    if (event_fence != nullptr)
    {
        CloseHandle(event_fence);
        event_fence = nullptr;
    }

    SAFE_RELEASE(fence_);
    SAFE_RELEASE(heapRTV_);
    for (int i = 0; i < FrameAmmount; i++) {
        SAFE_RELEASE(colbuf_[i]);
        SAFE_RELEASE(cmdalloc_[i]);
    }
    SAFE_RELEASE(cmdlist_);
    SAFE_RELEASE(cmdque_);
    SAFE_RELEASE(swpchain_);

    SAFE_RELEASE(device_);
}

void D3d::Run(int interval)
{
	write();
	waitGPU();
	present(interval);
}

D3d::~D3d()
{
}

D3d::D3d()
{
}

void D3d::write()
{
    cmdalloc_[IND_frame]->Reset();
    cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);

    D3D12_RESOURCE_BARRIER brr = {};
    {
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        brr.Transition.pResource = colbuf_[IND_frame];
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    cmdlist_->ResourceBarrier(1, &brr);

    float backcolor_[2][4] = { {0,0.6,0.5,1} ,{1,0.6,0.5,1} };

    cmdlist_->OMSetRenderTargets(1, &h_RTV[IND_frame], FALSE, nullptr);
    cmdlist_->ClearRenderTargetView(h_RTV[IND_frame], backcolor_[IND_frame ], 0, nullptr);

    {
        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    }

    cmdlist_->ResourceBarrier(1, &brr);
    cmdlist_->Close();


    ID3D12CommandList* commands[] = { cmdlist_ };
    cmdque_->ExecuteCommandLists(1, commands);
}

void D3d::waitGPU()
{
    assert(cmdque_ != nullptr);
    assert(fence_ != nullptr);
    assert(event_fence != nullptr);

    cmdque_->Signal(fence_, fencecnt_[IND_frame]);

    fence_->SetEventOnCompletion(fencecnt_[IND_frame], event_fence);

    WaitForSingleObjectEx(event_fence, INFINITE, FALSE);

    fencecnt_[IND_frame]++;
}

void D3d::present(uint32_t itv)
{
    swpchain_->Present(itv, 0);

    const auto curval = fencecnt_[IND_frame];
    cmdque_->Signal(fence_, curval);

    IND_frame = swpchain_->GetCurrentBackBufferIndex();

    if (fence_->GetCompletedValue() < fencecnt_[IND_frame])
    {
        fence_->SetEventOnCompletion(fencecnt_[IND_frame], event_fence);
        WaitForSingleObjectEx(event_fence, INFINITE, FALSE);
    }

    fencecnt_[IND_frame] = curval + 1;
}


