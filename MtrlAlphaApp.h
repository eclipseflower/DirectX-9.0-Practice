#pragma once
#include "InitApp.h"

struct Vertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	Vertex() {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		_x = x;
		_y = y;
		_z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
		_u = u;
		_v = v;
	}
};

class MtrlAlphaApp : public InitApp
{
public:
	ID3DXMesh *teapot = NULL;
	D3DMATERIAL9 teapotMtrl;
	IDirect3DVertexBuffer9 *bkQuad = NULL;
	IDirect3DTexture9 *bkTex = NULL;
	D3DMATERIAL9 bkMtrl;
	MtrlAlphaApp(int with, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
};