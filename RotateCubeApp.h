#pragma once
#include "InitApp.h"

struct Vertex 
{
	float _x, _y, _z;
	static const DWORD FVF = D3DFVF_XYZ;
	Vertex() {}
	Vertex(float x, float y, float z) 
	{
		_x = x;
		_y = y;
		_z = z;
	}
};

class RotateCubeApp : public InitApp 
{
public:
	IDirect3DVertexBuffer9 *vertexBuf = NULL;
	IDirect3DIndexBuffer9 *indexBuf = NULL;
	RotateCubeApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
};