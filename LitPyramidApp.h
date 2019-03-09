#pragma once
#include "InitApp.h"

struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz)
	{
		_x = x;
		_y = y;
		_z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
	}
};

class LitPyramidApp : public InitApp
{
public:
	IDirect3DVertexBuffer9 *pyramid = NULL;
	LitPyramidApp(int with, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
};