#pragma once
#include"DirectXMath.h"

using namespace DirectX;

struct VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 rgba;
};

struct alignas(256) WVP
{
	XMMATRIX wld;
	XMMATRIX view;
	XMMATRIX proj;
};

template <typename T>
struct CBUFFERVIEW
{
	T* ptr;
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
};

namespace SEMANTICS_STR
{
	LPCSTR POSITION = "POSITION";
	LPCSTR COLOR = "COLOR";
};