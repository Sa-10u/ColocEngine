#include"D3D.h"
#include"BUFFER.h"

bool D3d::Buffers::Initialize()
{
	bool FAILED = 0;
	bool res = FAILED;
  //  Vertex 
	VERTEX vx[] =
	{
		{XMFLOAT3(-1,-1,0),XMFLOAT4(0,0,1,1)},
		{XMFLOAT3( 1,-1,0),XMFLOAT4(0,1,0,1)},
		{XMFLOAT3( 0, 1,0),XMFLOAT4(1,0,0,1)}
	};
	//------------

	return true;
}