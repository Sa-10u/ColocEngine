#include "CE/BUFFER.h"
#include<assert.h>

VERTEX::VERTEX(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& uv, XMFLOAT3 const& tan):pos_(pos),norm_(norm),uv_(uv),tan_(tan)
{
}

MESH::MESH():ID_Material(-1),indexes_(),vtcs_()
{
}
//--------
