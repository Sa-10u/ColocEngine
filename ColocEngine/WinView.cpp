#include "WinView.h"
#include<cassert>
#define _THIS NULL
#define _ALL NULL
#define __guidof __uuidof

WinView::WinView(uint32_t h, uint32_t w) :h_ins(nullptr), h_wnd(nullptr), h_(h), w_(w)
{
}

WinView::~WinView()
{
}

void WinView::Run()
{
    if (setup())
    {
        loop();
    }

    termination();
}

//-------------------------------------------------------------------------------------

bool WinView::setup()
{
    if (initialize())
    {
        return true;
    }
    
    return false;
}

bool WinView::initialize()
{
    bool FAIL = 0;

    WNDCLASSEX wcex = {};

    h_ins = GetModuleHandle(_THIS);  //<----

    if (h_ins == nullptr) return FAIL;

    {
        auto color = CreateSolidBrush(RGB(1, 10, 100));

        wcex.lpfnWndProc = WndProc;
        wcex.lpszClassName = WND_NAME::smp;
        wcex.lpszMenuName = nullptr;
        wcex.cbSize = sizeof(wcex);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.hIcon = LoadIcon(h_ins, IDI_QUESTION);
        wcex.hIconSm = LoadIcon(h_ins, IDI_EXCLAMATION);
        wcex.hCursor = LoadCursor(h_ins, IDC_ARROW);
        wcex.hInstance = h_ins;
        wcex.hbrBackground = color;

        color = nullptr;
    }
    auto rgc = RegisterClassEx(&wcex);

    if(!rgc) return FAIL;

    RECT box = { 0,0,0,0 };
    {
        box.right = static_cast<LONG>(w_);
        box.bottom = static_cast<LONG>(h_);
    }

    {
        auto style = WS_OVERLAPPEDWINDOW;
        AdjustWindowRect(&box, style, false);

        h_wnd = CreateWindowEx
        (
            WS_EX_LAYERED,
            WND_NAME::smp,
            WND_NAME::smp,
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            (box.right - box.left),
            (box.bottom - box.top),
            nullptr,
            nullptr,
            h_ins,
            0
        );
    }

    if (h_wnd == nullptr)    return FAIL;

    ShowWindow(h_wnd,SW_SHOWNORMAL);
    UpdateWindow(h_wnd);
    SetFocus(h_wnd);

    return 1;
}

void WinView::termination()
{
    if (h_ins != nullptr)
    {
        UnregisterClass(WND_NAME::smp, h_ins);
    }

    h_ins = nullptr;
    h_wnd = nullptr;
}

void WinView::loop()
{
    MSG msg = {};

    while (msg.message != WM_QUIT) {

        if (PeekMessage(&msg, _ALL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {

        }
    }
}

bool WinView::initialize_D3D()
{
    bool FAIL = 0;

    auto res = D3D12CreateDevice
    (
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
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
    res = CreateDXGIFactory1(__guidof(fact), reinterpret_cast<void**>(&fact));
    if (FAILED(res))     return 0;


    DXGI_SWAP_CHAIN_DESC desc = {};
    {
    desc.BufferDesc.Height = h_;
    desc.BufferDesc.Width = w_;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = FrameAmmount;
    desc.OutputWindow = h_wnd;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    }
    IDXGISwapChain* p_swch = nullptr;
    res = fact->CreateSwapChain(cmdque_, &desc, &p_swch);
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


    for (int i = 0u;i < FrameAmmount; i++) {
        
        res = device_->CreateCommandAllocator
        (
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            __guidof(cmdalloc_[i]),
            reinterpret_cast<void**>(cmdalloc_[i])
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
}



void WinView::termination_D3D()
{
}

void WinView::rendaring()
{
    cmdalloc_[IND_frame]->Reset();
    cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);
}

void WinView::waitGPU()
{
}

void WinView::present(uint32_t itv)
{
}

LRESULT WinView::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:PostQuitMessage(0);     break;

    default: return DefWindowProc(hwnd,msg,wp,lp);           break;

    }
}