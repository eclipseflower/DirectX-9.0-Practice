#pragma once
#include "InitApp.h"
class ToonEffectApp : public InitApp
{
public:
	ToonEffectApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	~ToonEffectApp();

	ID3DXMesh*  meshes[4] = { NULL, NULL, NULL, NULL };
	D3DXMATRIX  worldMatrices[4];
	D3DXVECTOR4 meshColors[4];
	ID3DXEffect *toonEffect = NULL;

	D3DXHANDLE worldViewHandle = 0;
	D3DXHANDLE worldViewProjHandle = 0;
	D3DXHANDLE colorHandle = 0;
	D3DXHANDLE lightDirHandle = 0;
	D3DXHANDLE shadeTexHandle = 0;
	D3DXHANDLE toonTechHandle = 0;

	D3DXMATRIX projMatrix;
};

