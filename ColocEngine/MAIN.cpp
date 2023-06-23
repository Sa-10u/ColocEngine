#include "WinView.h"

int main(int argc,wchar_t** argv,wchar_t** evnp)
{
	
	WinView WND(9*50, 16*50);
	
	WND.Run();

	return 0;
}