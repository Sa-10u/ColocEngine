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
    res =  CreateDXGIFactory1(__guidof(fact), reinterpret_cast<void**>(&fact));
    if (FAILED(res))     return 0;


    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferDesc.Height = h_;
    desc.BufferDesc.Width = w_;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;

}

void WinView::termination_D3D()
{
}

void WinView::rendaring()
{
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