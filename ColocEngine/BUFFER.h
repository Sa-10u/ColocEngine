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