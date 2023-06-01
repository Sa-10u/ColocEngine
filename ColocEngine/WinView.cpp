#include "WinView.h"
#define _THIS NULL
#define _ALL NULL

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

    HINSTANCE hIns = nullptr;
    WNDCLASSEX wcex = {};

    hIns = GetModuleHandle(_THIS);
    if (hIns == nullptr) return FAIL;

    {
        auto color = (HBRUSH)(RGB(1,1,1));

        wcex.lpfnWndProc = WndProc;
        wcex.lpszClassName = WND_NAME::smp;
        wcex.lpszMenuName = nullptr;
        wcex.cbSize = sizeof(wcex);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.hIcon = LoadIcon(hIns, IDI_QUESTION);
        wcex.hIconSm = LoadIcon(hIns, IDI_EXCLAMATION);
        wcex.hCursor = LoadCursor(hIns, IDC_ARROW);
        wcex.hInstance = hIns;
        wcex.hbrBackground = color;
    }

    if(!RegisterClassEx(&wcex)) return FAIL;

    RECT box = { 0,0,0,0 };
    {
        box.right = static_cast<LONG>(w_);
        box.bottom = static_cast<LONG>(h_);
    }

    {
        long style = WS_POPUPWINDOW;
        AdjustWindowRect(&box, style, false);

        h_wnd = CreateWindowEx
        (
            0,
            WND_NAME::smp,
            TEXT("What is this?"),
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            (box.right - box.left),
            (box.bottom - box.top),
            nullptr,
            nullptr,
            h_ins,
            nullptr

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
    }
}

LRESULT WinView::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:PostQuitMessage(0);     break;

    default: return DefWindowProc(hwnd,msg,wp,lp);           break;

    }
}