#include"D3D.h"
#include"BUFFER.h"
#include"MACRO.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

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

	HRESULT res = parent_->device_->CreateCommittedResource
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
	res = parent_->device_->CreateDescriptorHeap(&hpcb, __guidof(heapCBV_), reinterpret_cast<void**>(&heapCBV_));

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

		res = parent_->device_->CreateCommittedResource
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

		{
			auto incre = parent_->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			auto adr = CB[i]->GetGPUVirtualAddress();
			auto h_CPU = heapCBV_->GetCPUDescriptorHandleForHeapStart();
			auto h_GPU = heapCBV_->GetGPUDescriptorHandleForHeapStart();

			h_CPU.ptr += incre * i;
			h_GPU.ptr += incre * i;

			CBV[i].HCPU = h_CPU;
			CBV[i].HGPU = h_GPU;
			CBV[i].desc.BufferLocation = adr;
			CBV[i].desc.SizeInBytes = sizeof(WVP);

			parent_->device_->CreateConstantBufferView(&CBV[i].desc, CBV[i].HCPU);

			{
				res = CB[i]->Map(NULL, nullptr, reinterpret_cast<void**>(&CBV[i]));
				if (FAILED(res))    return FAIL;


				{
					auto Cam_pos = XMVectorSet(0, 0, 5, 0);
					auto Tgt_pos = XMVectorZero();
					auto Upward = XMVectorSet(0, 1, 0, 0);

					float FOV = 37.5 * (_PI / 180);
					float Aspect = parent_->Width / parent_->Height;

					CBV[i].buffer->wld = XMMatrixIdentity();
					CBV[i].buffer->proj = XMMatrixLookAtRH(Cam_pos, Tgt_pos, Upward);
					CBV[i].buffer->view = XMMatrixPerspectiveFovRH(FOV, Aspect, 1.0f, 100.0f);
				}
			}

		}
	}

	res = ProcInputLayout_();
	if (FAILED(res))	return 0;

	res = MakePipelineState_();
	if (FAILED(res))	return 0;

	return true;
}

void D3d::Buffers::Termination()
{
	for (int i = 0; i < FrameAmmount; i++) {

		if (&CB[i] != nullptr) {
			CB[i]->Unmap(NULL, 0);
			memset(&CBV[i], 0,sizeof(CBV[i]));
		}

		CB[i]->Release();
		//statePipe_->Release();
	}

	VB->Release();
	statePipe_->Release();

}

void D3d::Buffers::SetParent(D3d* parent)
{
	this->parent_ = parent;
}

bool D3d::Buffers::ProcInputLayout_()
{
	
	D3D12_ROOT_PARAMETER param = {};
	{
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		param.Descriptor.ShaderRegister = 0;
		param.Descriptor.RegisterSpace = UINT_MAX;
		param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	}

	D3D12_ROOT_SIGNATURE_DESC rsdesc = {};
	{
		rsdesc.NumParameters = 1;
		rsdesc.pParameters = &param;
		rsdesc.NumStaticSamplers = 0;
		rsdesc.pStaticSamplers = nullptr;
		rsdesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | 
						D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
						D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | 
						D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	}

	ID3DBlob* blob, *erblob;
	auto res = D3D12SerializeRootSignature
	(
		&rsdesc,
		D3D_ROOT_SIGNATURE_VERSION_1_1,
		&blob,
		&erblob
	);
	if (FAILED(res))	return 0;

	res = parent_->device_->CreateRootSignature
	(
		0,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		__guidof(blob),
		reinterpret_cast<void**>(&blob)
	);
	if (FAILED(res))	return 0;

	return 1;
}

bool D3d::Buffers::MakePipelineState_()
{
	D3D12_INPUT_ELEMENT_DESC iedesc[2] = {};
	{
		iedesc[0].SemanticName = "POSITION";
		iedesc[0].SemanticIndex = 0;
		iedesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		iedesc[0].InputSlot = 0;
		iedesc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		iedesc[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		iedesc[0].InstanceDataStepRate = 0;
	}
	{
		iedesc[1].SemanticName = "COLOR";
		iedesc[1].SemanticIndex = 0;
		iedesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		iedesc[1].InputSlot = 0;
		iedesc[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		iedesc[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		iedesc[1].InstanceDataStepRate = 0;
	}
	//--------------

	D3D12_RASTERIZER_DESC rasdesc = {};
	{
		rasdesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasdesc.CullMode = D3D12_CULL_MODE_BACK;
		rasdesc.FrontCounterClockwise = false;
		rasdesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasdesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasdesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasdesc.DepthClipEnable = false;
		rasdesc.MultisampleEnable = false;
		rasdesc.AntialiasedLineEnable = false;
		rasdesc.ForcedSampleCount = 0;
		rasdesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	D3D12_RENDER_TARGET_BLEND_DESC descrtb = {};	//desc of desc . lol
	{
		descrtb.BlendEnable = false;
		descrtb.LogicOpEnable = false;
		descrtb.SrcBlend = D3D12_BLEND_ONE;
		descrtb.DestBlend = D3D12_BLEND_ZERO;
		descrtb.BlendOp = D3D12_BLEND_OP_ADD;
		descrtb.SrcBlendAlpha = D3D12_BLEND_ONE;
		descrtb.DestBlendAlpha = D3D12_BLEND_ZERO;
		descrtb.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		descrtb.LogicOp = D3D12_LOGIC_OP_NOOP;
		descrtb.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}

	D3D12_BLEND_DESC descblend = {};
	{
		descblend.AlphaToCoverageEnable = false;
		descblend.IndependentBlendEnable = false;
		
		for (auto i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			descblend.RenderTarget[i] = descrtb;
		}
	}


	ID3DBlob* blobVS;
	ID3DBlob* blobPS;

	auto res = D3DReadFileToBlob(L"PoorVS.cso", &blobVS);
	if (FAILED(res))	return 0;

	auto res = D3DReadFileToBlob(L"PoorPS.cso", &blobPS);
	if (FAILED(res))	return 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC descpipest = {};
	{
		descpipest.InputLayout = {iedesc, _countof(iedesc)};
		descpipest.pRootSignature = rootsig_;
		descpipest.VS = { blobVS->GetBufferPointer(),blobVS->GetBufferSize() };
		descpipest.PS = { blobPS->GetBufferPointer(),blobPS->GetBufferSize() };
		descpipest.RasterizerState = rasdesc;
		descpipest.BlendState = descblend;
		descpipest.DepthStencilState.DepthEnable = false;
		descpipest.DepthStencilState.StencilEnable = false;
		descpipest.SampleMask = UINT_MAX;
		descpipest.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		descpipest.NumRenderTargets = 1;
		descpipest.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		descpipest.DSVFormat = DXGI_FORMAT_UNKNOWN;
		descpipest.SampleDesc.Count = 1;
		descpipest.SampleDesc.Quality = NULL;
	}

	res = parent_->device_->CreateComputePipelineState
}
