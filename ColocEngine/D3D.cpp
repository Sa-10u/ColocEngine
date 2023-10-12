#include "D3D.h"
#include<cassert>


void D3d::Kill()
{
    delete me;
    me = nullptr;
}

bool D3d::Initialize(HWND hwnd, uint32_t h, uint32_t w)
{
#if 1
    ID3D12Debug* db;
    D3D12GetDebugInterface(IID_PPV_ARGS(&db));

    db->EnableDebugLayer();

#endif


    this->Height = h;
    this->Width = w;

    bool FAIL = 0;

    auto res = D3D12CreateDevice
    (
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        __guidof(device_),
        reinterpret_cast<void**>(&device_)
    );
    if (FAILED(res))               return FAIL;


    D3D12_COMMAND_QUEUE_DESC cmddesc = {};

    {
        cmddesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        cmddesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        cmddesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        cmddesc.NodeMask = NULL;
    }
    res = device_->CreateCommandQueue(&cmddesc, __guidof(cmdque_), reinterpret_cast<void**>(&cmdque_));
    if (FAILED(res))     return FAIL;


    IDXGIFactory4* fact = nullptr;
    res = CreateDXGIFactory1(__guidof(fact), reinterpret_cast<void**>(&fact));
    if (FAILED(res))     return 0;


    DXGI_SWAP_CHAIN_DESC desc = {};
    {
        desc.BufferDesc.Height = Height;
        desc.BufferDesc.Width = Width;
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = FrameAmmount;
        desc.OutputWindow = hwnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    }
    IDXGISwapChain* p_swch = nullptr;
    res = fact->CreateSwapChain(cmdque_, &desc, &p_swch); 
    if (FAILED(res))
    {
        SAFE_RELEASE(fact);
        return FAIL;
    }

    res = p_swch->QueryInterface(__guidof(swpchain_), reinterpret_cast<void**>(&swpchain_));
    if (FAILED(res))
    {
        SAFE_RELEASE(fact);
        SAFE_RELEASE(p_swch);
        return FAIL;
    }

    IND_frame = swpchain_->GetCurrentBackBufferIndex();
    SAFE_RELEASE(fact);
    SAFE_RELEASE(p_swch);


    for (int i = 0u; i < FrameAmmount; i++) {

        res = device_->CreateCommandAllocator
        (
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            __guidof(cmdalloc_[i]),
            reinterpret_cast<void**>(&cmdalloc_[i])
        );

        if (FAILED(res)) return FAIL;
    }

    res = device_->CreateCommandList
    (
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmdalloc_[IND_frame],
        nullptr,
        __guidof(cmdlist_),
        reinterpret_cast<void**>(&cmdlist_)
    );
    if (FAILED(res)) return FAIL;

    D3D12_DESCRIPTOR_HEAP_DESC hpdesc;

    {
        hpdesc.NumDescriptors = FrameAmmount;
        hpdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        hpdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        hpdesc.NodeMask = 0;
    }

    res = device_->CreateDescriptorHeap(&hpdesc, __guidof(heapRTV_), reinterpret_cast<void**>(&heapRTV_));

    if (FAILED(res))    return FAIL;


    D3D12_CPU_DESCRIPTOR_HANDLE handle = heapRTV_->GetCPUDescriptorHandleForHeapStart();
    UINT incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    for (UINT i = 0u; i < FrameAmmount; i++) {

        res = swpchain_->GetBuffer(i, IID_PPV_ARGS(&colbuf_[i]));
        if (FAILED(res))     return FAIL;

        D3D12_RENDER_TARGET_VIEW_DESC rtvdesc = {};

        rtvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        rtvdesc.Texture2D.MipSlice = NULL;
        rtvdesc.Texture2D.PlaneSlice = NULL;

        device_->CreateRenderTargetView(colbuf_[i], &rtvdesc, handle);

        h_RTV[i] = handle;
        handle.ptr += incre;
    }

    res = device_->CreateFence
    (
        fencecnt_[IND_frame],
        D3D12_FENCE_FLAG_NONE,
        __guidof(fence_),
        reinterpret_cast<void**>(&fence_)
    );

    if (FAILED(res))     return FAIL;

    event_fence = CreateEvent(nullptr, false, false, nullptr);
    if (event_fence == nullptr)  return FAIL;

    if (!InitPoly()) return false;

    return true;
}

bool D3d::InitPoly()
{
    HRESULT res = FALSE;

    VERTEX vts[] = 
    {
        {XMFLOAT3(-1.0f,-1.0f,0.0f),XMFLOAT4(0.0f,0.0f,1.0f,1.0f)},
        {XMFLOAT3(1.0f,-1.0f,0.0f),XMFLOAT4(0.0f,1.0f,0.0f,1.0f)},
        {XMFLOAT3(0.0f,-1.0f,0.0f),XMFLOAT4(1.0f,0.0f,0.0f,1.0f)},
    };

    {
        D3D12_HEAP_PROPERTIES hp_prop_v = {};
        {
            hp_prop_v.Type = D3D12_HEAP_TYPE_UPLOAD;
            hp_prop_v.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_prop_v.CreationNodeMask = 1;
            hp_prop_v.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            hp_prop_v.VisibleNodeMask = 1;
        };

        D3D12_RESOURCE_DESC rc_desc_v = {};
        {
            rc_desc_v.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            rc_desc_v.Alignment = 0;
            rc_desc_v.Width = sizeof(vts);
            rc_desc_v.Height = 1;
            rc_desc_v.DepthOrArraySize = 1;
            rc_desc_v.MipLevels = 1;
            rc_desc_v.Format = DXGI_FORMAT_UNKNOWN;
            rc_desc_v.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            rc_desc_v.Flags = D3D12_RESOURCE_FLAG_NONE;

            rc_desc_v.SampleDesc.Count = 1;
            rc_desc_v.SampleDesc.Quality = 0;
        }

        res = device_->CreateCommittedResource
        (
            &hp_prop_v,
            D3D12_HEAP_FLAG_NONE,
            &rc_desc_v,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&VB)
        );
    }

    if (FAILED(res))  return false;
    //----------------------------
    {

        void* ptr = nullptr;
        res = VB->Map(NULL, 0, &ptr);

        if (FAILED(res)) return false;

        memcpy(ptr, vts, sizeof(vts));

        VB->Unmap(0, 0);

        VBV.BufferLocation = VB->GetGPUVirtualAddress();
        VBV.SizeInBytes = sizeof(vts);
        VBV.StrideInBytes = sizeof(VERTEX);
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC hp_desc = {};
        {
            hp_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            hp_desc.NodeMask = 0;
            hp_desc.NumDescriptors = 1 * FrameAmmount;
            hp_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        }

        res = device_->CreateDescriptorHeap
        (
            &hp_desc,
            IID_PPV_ARGS(&heapCBV_)
        );

        if (FAILED(res)) return false;
    }

    //------------------------
    {
        D3D12_HEAP_PROPERTIES hp_proc_c;
        {
            hp_proc_c.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            hp_proc_c.Type = D3D12_HEAP_TYPE_UPLOAD;
            hp_proc_c.CreationNodeMask = 1;
            hp_proc_c.VisibleNodeMask = 1;
            hp_proc_c.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        }

        D3D12_RESOURCE_DESC rc_desc_c;
        {
            rc_desc_c.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            rc_desc_c.Format = DXGI_FORMAT_UNKNOWN;
            rc_desc_c.MipLevels = 1;
            rc_desc_c.Alignment = 0;
            rc_desc_c.Height = 1;
            rc_desc_c.Width = sizeof(WVP);
            rc_desc_c.DepthOrArraySize = 1;
            rc_desc_c.Flags = D3D12_RESOURCE_FLAG_NONE;
            rc_desc_c.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            rc_desc_c.SampleDesc.Count = 1;
            rc_desc_c.SampleDesc.Quality = 0;
        }

        for (auto i = 0; i < FrameAmmount;i++) {

            res = device_->CreateCommittedResource
            (
                &hp_proc_c,
                D3D12_HEAP_FLAG_NONE,
                &rc_desc_c,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                NULL,
                IID_PPV_ARGS(&CB[i])
            );
            //----------------------------------

            static auto incre = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            auto address = CB[i]->GetGPUVirtualAddress();
            auto _HCPU = heapCBV_->GetCPUDescriptorHandleForHeapStart();
            auto _HGPU = heapCBV_->GetGPUDescriptorHandleForHeapStart();

            _HCPU.ptr += incre * i;
            _HGPU.ptr += incre * i;

            CBV[i].HCPU = _HCPU;
            CBV[i].HGPU = _HGPU;
            CBV[i].desc.BufferLocation = address;
            CBV[i].desc.SizeInBytes = sizeof(WVP);

            device_->CreateConstantBufferView(&CBV[i].desc, CBV[i].HCPU);
            res = CB[i]->Map(0, NULL, reinterpret_cast<void**>(&CBV[i].ptr));
            if (FAILED(res)) return 0;

            auto C_pos = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
            auto C_tgt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
            auto C_head = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            auto C_fovY = 37.5 * (_PI / 180);
            auto C_aspect = Width / Height;

            CBV[i].ptr->wld = XMMatrixIdentity();
            CBV[i].ptr->view = XMMatrixLookAtRH(C_pos, C_tgt, C_head);
            CBV[i].ptr->proj = XMMatrixPerspectiveFovRH(C_fovY, C_aspect, 1.0f ,100.0f);

        }
    }
    //-----------------------------------------------------------
    {
        D3D12_ROOT_PARAMETER r_param;
        {
            r_param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            r_param.Descriptor.RegisterSpace = 0;
            r_param.Descriptor.ShaderRegister = 0;
            r_param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
        }

        D3D12_ROOT_SIGNATURE_DESC r_s_desc;
        {
            r_s_desc.pParameters = &r_param;
            r_s_desc.pStaticSamplers = nullptr;
            r_s_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
            r_s_desc.NumParameters = 1;
            r_s_desc.NumStaticSamplers = 0;
        }

        ID3DBlob* S_blob;
        ID3DBlob* E_blob;

        res = D3D12SerializeRootSignature
        (
            &r_s_desc,
            D3D_ROOT_SIGNATURE_VERSION_1_1,
            &S_blob,
            &E_blob
        );

        res = device_->CreateRootSignature
        (
            NULL,
            S_blob->GetBufferPointer(),
            S_blob->GetBufferSize(),
            IID_PPV_ARGS(&rootsig_)
        );

        if (FAILED(res))     return 0;
    }
    //---------------------------------
    {
        D3D12_INPUT_ELEMENT_DESC in_e_desc[2];
        { 
            {
                in_e_desc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                in_e_desc[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
                in_e_desc[0].InputSlot = 0;
                in_e_desc[0].SemanticName = SEMANTICS_STR::POSITION;
                in_e_desc[0].SemanticIndex = 0;
                in_e_desc[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                in_e_desc[0].InstanceDataStepRate = 0;
            }
            {
                in_e_desc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                in_e_desc[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
                in_e_desc[1].InputSlot = 0;
                in_e_desc[1].SemanticName = SEMANTICS_STR::COLOR;
                in_e_desc[1].SemanticIndex = 0;
                in_e_desc[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                in_e_desc[1].InstanceDataStepRate = 0;
            }
        }
        //------------------------------

        D3D12_RASTERIZER_DESC rs_desc;
        {
            rs_desc.FillMode = D3D12_FILL_MODE_SOLID;
            rs_desc.CullMode = D3D12_CULL_MODE_NONE;
            rs_desc.FrontCounterClockwise = false;
            rs_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
            rs_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
            rs_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
            rs_desc.AntialiasedLineEnable = false;
            rs_desc.DepthClipEnable = false;
            rs_desc.MultisampleEnable = false;
            rs_desc.ForcedSampleCount = 0;
            rs_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        }


    }

}

void D3d::Termination()
{

    waitGPU();
    if (event_fence != nullptr)
    {
        CloseHandle(event_fence);
        event_fence = nullptr;
    }

    SAFE_RELEASE(fence_);
    SAFE_RELEASE(heapRTV_);
    for (int i = 0; i < FrameAmmount; i++) {
        SAFE_RELEASE(colbuf_[i]);
        SAFE_RELEASE(cmdalloc_[i]);
    }
    SAFE_RELEASE(cmdlist_);
    SAFE_RELEASE(cmdque_);
    SAFE_RELEASE(swpchain_);

    SAFE_RELEASE(device_);
    

}

void D3d::Run(int interval)
{

	write();
	waitGPU();

    {
        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    }

    cmdlist_->ResourceBarrier(1, &brr);
    cmdlist_->Close();


    ID3D12CommandList* commands[] = { cmdlist_ };
    cmdque_->ExecuteCommandLists(1, commands);
	present(interval);

    
}

void D3d::SetHeight(float h)
{
    Height = h;
}

void D3d::SetWidth(float w)
{
    Width = w;
}

float D3d::GetHeight()
{
    return Height;
}

float D3d::GetWidth()
{
    return Width;
}

D3d::~D3d()
{
}

D3d::D3d()
{
    Height = 0.0f;
    Width = 0.0f;
}

void D3d::write()
{
    cmdalloc_[IND_frame]->Reset();
    cmdlist_->Reset(cmdalloc_[IND_frame], nullptr);

    {
        brr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        brr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        brr.Transition.pResource = colbuf_[IND_frame];
        brr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        brr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        brr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    }
    cmdlist_->ResourceBarrier(1, &brr);

    float backcolor_[2][4] = { {0,0.6,0.5,1} ,{1,0.6,0.5,1} };

    cmdlist_->OMSetRenderTargets(1, &h_RTV[IND_frame], FALSE, nullptr);
    cmdlist_->ClearRenderTargetView(h_RTV[IND_frame], backcolor_[IND_frame], 0, nullptr);

}

void D3d::waitGPU()
{
    assert(cmdque_ != nullptr);
    assert(fence_ != nullptr);
    assert(event_fence != nullptr);

    cmdque_->Signal(fence_, fencecnt_[IND_frame]);

    fence_->SetEventOnCompletion(fencecnt_[IND_frame], event_fence);

    WaitForSingleObjectEx(event_fence, INFINITE, FALSE);

    fencecnt_[IND_frame]++;
}

void D3d::present(uint32_t itv)
{
    swpchain_->Present(itv, 0);

    const auto curval = fencecnt_[IND_frame];
    cmdque_->Signal(fence_, curval);

    IND_frame = swpchain_->GetCurrentBackBufferIndex();

    if (fence_->GetCompletedValue() < fencecnt_[IND_frame])
    {
        fence_->SetEventOnCompletion(fencecnt_[IND_frame], event_fence);
        WaitForSingleObjectEx(event_fence, INFINITE, FALSE);
    }

    fencecnt_[IND_frame] = curval + 1;
}


