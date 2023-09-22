#pragma once
#include<DirectXMath.h>

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

constexpr double _PI = 3.141592;


typedef DirectX::XMFLOAT2 fl2;
typedef DirectX::XMFLOAT3 fl3;
typedef DirectX::XMFLOAT4 fl4;

typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMVECTOR Vec;
