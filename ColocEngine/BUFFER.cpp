#include "BUFFER.h"
#include<assert.h>
#include<codecvt>

VERTEX::VERTEX():pos_(0),norm_(0),uv_(0),tan_(0)
{
}

VERTEX::VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT3 const& uv, XMFLOAT3 const& tan):pos_(pos),norm_(norm),uv_(uv),tan_(tan)
{
}

MATERIAL::MATERIAL():dif_(0),spec_(0),alpha_(0),shin_(0),map_(0)
{
}

MESH::MESH():vtcs_(nullptr),indexes_(nullptr),ID_Material(-1)
{
}

bool MESH::LoadMesh(const wchar_t* file, MESH& mesh, MATERIAL& material)
{
	return false;
}
