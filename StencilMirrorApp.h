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

class StencilMirrorApp : public InitApp
{
public:
	IDirect3DVertexBuffer9 *vertexBuf = NULL;
	D3DMATERIAL9 floorMtrl;
	IDirect3DTexture9 *floorTex = NULL;
	D3DMATERIAL9 wallMtrl;
	IDirect3DTexture9 *wallTex = NULL;
	D3DMATERIAL9 mirrorMtrl;
	IDirect3DTexture9 *mirrorTex = NULL;
	D3DXVECTOR3 teapotPosition;
	D3DMATERIAL9 teapotMtrl;
	ID3DXMesh *teapot = NULL;
	StencilMirrorApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
private:
	void RenderScene();
	void RenderMirror();
};