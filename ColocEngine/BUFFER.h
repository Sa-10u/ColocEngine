#pragma once
#include"DirectXMath.h"
#include<d3d12.h>
#include <string>
#include <vector>

using std::string;
using std::vector;
using namespace DirectX;

struct SIMPLEVERTEX
{
	XMFLOAT3 pos  ;
	XMFLOAT2 uv ;
};

struct VERTEX
{
	XMFLOAT3 pos_;
	XMFLOAT3 norm_;
	XMFLOAT3 uv_;
	XMFLOAT3 tan_;

	static const D3D12_INPUT_LAYOUT_DESC inp_Layout;

	VERTEX();
	VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT3 const& uv, XMFLOAT3 const& tan);


	enum ELEMENT
	{
		POSITION = 0,
		NORMAL,
		UV,
		TANGENT,

		AMMOUNT
	};

private:

	static const int count = 4;
	static const D3D12_INPUT_ELEMENT_DESC element[AMMOUNT];
};

//-------------

struct Texture
{
	ID3D12Resource* rsc_ptr = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
};

struct MATERIAL
{
	XMFLOAT3 dif_;
	XMFLOAT3 spec_;
	float alpha_;
	float shin_;
	string map_;

	MATERIAL();
};

struct MESH
{
	vector<VERTEX> vtcs_;
	vector<uint32_t> indexes_;

	uint32_t ID_Material;

	MESH();
	//-----
	bool LoadMesh(const wchar_t* file, vector<MESH>& mesh, vector<MATERIAL>& material);
};

//-----------------------------

struct alignas(256) WVPT
{
	XMMATRIX wld;
	XMMATRIX view;
	XMMATRIX proj;
	float time;
};

template <typename T>
struct CBUFFERVIEW
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	D3D12_CPU_DESCRIPTOR_HANDLE HCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE HGPU;
	T* ptr = nullptr;
};

