#pragma once

template<typename tgt>
inline void SAFE_DELETE(tgt*& p)
{
	if (!(p == nullptr)) 
	{
		delete p;
		p = nullptr;
	}
}

template<typename tgt>
inline void SAFE_RELEASE(tgt*& p)
{
	if (!(p == nullptr))
	{
		p->Release();
		p = nullptr;
	}

}

#define __guidof __uuidof

