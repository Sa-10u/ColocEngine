#pragma once
#include <Windows.h>
#include <cstdint>

class WinView
{
public:

	WinView(uint32_t h, uint32_t w);
	~WinView();
	void Run();
	
private:

	HINSTANCE h_ins;
	HWND h_wnd;
	uint32_t h_;
	uint32_t w_;

private:

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	bool setup();
	bool initialize();
	void termination();
	void loop();

};

namespace WND_NAME
{
	const auto smp = TEXT("Sample");
}

