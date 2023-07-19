#include"D3D.h"
#include"BUFFER.h"
#include"MACRO.h"

bool D3d::Buffers::Initialize()
{
	bool FAIL = 0;
  //  Vertex 
	VERTEX vx[] =
	{
		{fl3(-1,-1,0),fl4(0,0,1,1)},
		{fl3( 1,-1,0),fl4(0,1,0,1)},
		{fl3( 0, 1,0),fl4(1,0,0,1)}
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
	if (FAILED(res))	return FAIL;

	void* ptr = nullptr;
	VB->Map(0, nullptr, &ptr);
	memcpy(ptr, vx, sizeof(vx));
	VB->Unmap(0, nullptr);

	VBV.BufferLocation = VB->GetGPUVirtualAddress();
	VBV.SizeInBytes = sizeof(vx);
	VBV.StrideInBytes = sizeof(VERTEX);

	D3D12_DESCRIPTOR_HEAP_DESC hpcb;

	{
		hpcb.NumDescriptors = FrameAmmount;
		hpcb.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		hpcb.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		hpcb.NodeMask = 0;
	}
	res = D3d::instance->device_->CreateDescriptorHeap(&hpcb, __guidof(heapCBV_), reinterpret_cast<void**>(&heapCBV_));

	if (FAILED(res))    return FAIL;
	
	D3D12_HEAP_PROPERTIES cbprop = {};
	{
		cbprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		cbprop.CreationNodeMask = 1;
		cbprop.VisibleNodeMask = 1;
		cbprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		cbprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	}
	D3D12_RESOURCE_DESC cbdesc = {};
	{
		cbdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbdesc.Height = 1;
		cbdesc.Width = sizeof(WVP);
		cbdesc.DepthOrArraySize = 1;

		cbdesc.Format = DXGI_FORMAT_UNKNOWN;
		cbdesc.MipLevels = 1;
		cbdesc.Alignment = 0;
		cbdesc.Format = DXGI_FORMAT_UNKNOWN;
		cbdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		cbdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		cbdesc.SampleDesc.Quality = 0;
		cbdesc.SampleDesc.Count = 1;
	}

	for (int i = 0;i < FrameAmmount;i++) {

		res = D3d::instance->device_->CreateCommittedResource
		(
			&cbprop,
			D3D12_HEAP_FLAG_NONE,
			&cbdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			__guidof(CB[i]),
			reinterpret_cast<void**>(&CB[i])
		);

		if (FAILED(res))    return FAIL;
	}

	return true;
}