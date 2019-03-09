#pragma once
#include "InitApp.h"

struct MultiTexVertex
{
	MultiTexVertex(float x, float y, float z, float u0, float v0, float u1, float v1, float u2, float v2)
	{
		_x = x; _y = y; _z = z;
		_u0 = u0; _v0 = v0;
		_u1 = u1; _v1 = v1;
		_u2 = u2; _v2 = v2;

	}
	float _x, _y, _z;
	float _u0, _v0;
	float _u1, _v1;
	float _u2, _v2;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX3;
};

class PSMultiTexApp : public InitApp
{
public:
	PSMultiTexApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	~PSMultiTexApp();

	IDirect3DVertexBuffer9 *quadVB;
	ID3DXConstantTable *multiTexCT;
	IDirect3DPixelShader9 *multiTexPS;
	IDirect3DTexture9 *baseTex;
	IDirect3DTexture9 *spotLightTex;
	IDirect3DTexture9 *stringTex;
	D3DXHANDLE baseTexHandle;
	D3DXHANDLE spotLightTexHandle;
	D3DXHANDLE stringTexHandle;
	D3DXCONSTANT_DESC baseTexDesc;
	D3DXCONSTANT_DESC spotLightTexDesc;
	D3DXCONSTANT_DESC stringTexDesc;

};

