#pragma once
#include "InitApp.h"
class VSDiffuseApp : public InitApp
{
public:
	VSDiffuseApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	~VSDiffuseApp();

	IDirect3DVertexShader9 *diffuseShader = NULL;
	ID3DXConstantTable *diffuseConstantTable = NULL;

	ID3DXMesh *teapot = NULL;

	D3DXHANDLE viewMatrixHandle = 0;
	D3DXHANDLE viewProjMatrixHandle = 0;
	D3DXHANDLE ambientMtrlHandle = 0;
	D3DXHANDLE diffuseMtrlHandle = 0;
	D3DXHANDLE lightDirHandle = 0;

	D3DXMATRIX proj;
};

