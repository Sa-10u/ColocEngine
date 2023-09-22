#include "WinView.h"

#define _ALL NULL
#define _THIS NULL

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
    D3D = new D3d;

    if (initialize())
    {
        if (D3D->Initialize(h_wnd,h_,w_))
        {
            ChangeAspect();


            return true;
        }
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
            NULL,
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


    return true;
}

void WinView::termination()
{
    if (h_ins != nullptr)
    {
        D3D->Termination();
        D3D->Kill();

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
            D3D->Run(0);
        }
    }
}

void WinView::ChangeAspect()
{
    D3D->SetHeight(h_);
    D3D->SetWidth(w_);
}


LRESULT WinView::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_DESTROY:PostQuitMessage(0);     return 0;

    default: return DefWindowProc(hwnd,msg,wp,lp);           return 0;

    }
}