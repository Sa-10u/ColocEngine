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
	T* buffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle;
};