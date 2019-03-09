#pragma once
#include "InitApp.h"
#include <vector>

class LightTexEffectApp : public InitApp
{
public:
	LightTexEffectApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	~LightTexEffectApp();

	ID3DXMesh* mesh = NULL;
	std::vector<D3DMATERIAL9> mtrls;
	std::vector<IDirect3DTexture9*> textures;
	ID3DXEffect *lightTexEffect = NULL;

	D3DXHANDLE worldMatrixHandle = 0;
	D3DXHANDLE viewMatrixHandle = 0;
	D3DXHANDLE projMatrixHandle = 0;
	D3DXHANDLE texHandle = 0;
	D3DXHANDLE lightTexTechHandle = 0;
};

