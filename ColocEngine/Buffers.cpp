#include"D3D.h"
#include"BUFFER.h"
#include"MACRO.h"

bool D3d::Buffers::Initialize()
{
	bool FAILED = 0;
  //  Vertex 
	VERTEX vx[] =
	{
		{XMFLOAT3(-1,-1,0),XMFLOAT4(0,0,1,1)},
		{XMFLOAT3( 1,-1,0),XMFLOAT4(0,1,0,1)},
		{XMFLOAT3( 0, 1,0),XMFLOAT4(1,0,0,1)}
	};
	//------------

	D3D12_HEAP_PROPERTIES propheap_;
	{
		propheap_.Type = D3D12_HEAP_TYPE_UPLOAD;
		propheap_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		propheap_.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		propheap_.CreationNodeMask = 1;
		propheap_.VisibleNodeMask = 1;
	}

	D3D12_RESOURCE_DESC rdc_;
	{
		rdc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		rdc_.Alignment = 0;
		rdc_.Width = sizeof(VERTEX);
		rdc_.Height = 1;
		rdc_.DepthOrArraySize = 1;
		rdc_.MipLevels = 1;
		rdc_.Format = DXGI_FORMAT_UNKNOWN;
		rdc_.SampleDesc.Count = 1;
		rdc_.SampleDesc.Quality = 0;
		rdc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		rdc_.Flags = D3D12_RESOURCE_FLAG_NONE;
	}

	HRESULT res = D3d::instance->device_->CreateCommittedResource
	(
		&propheap_,
		D3D12_HEAP_FLAG_NONE,
		&rdc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		__guidof(VB),
		reinterpret_cast<void**>(&VB)
	);
	if (FAILED(res))	return FAILED;

	return true;
}