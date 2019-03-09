#pragma once
#include "InitApp.h"
#include <vector>

struct BoundingBox
{
	D3DXVECTOR3 _min, _max;
};

struct BoundingSphere
{
	D3DXVECTOR3 _center;
	float _radius;
};

class BoundingVolumeApp : public InitApp
{
public:
	ID3DXMesh *srcMesh = NULL;
	std::vector<D3DMATERIAL9> mtrls;
	std::vector<IDirect3DTexture9*> textures;
	ID3DXMesh *sphereMesh = NULL;
	ID3DXMesh *boxMesh = NULL;
	bool sphereMode = false;
	D3DMATERIAL9 blueMtrl;
	BoundingVolumeApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
private:
	bool ComputeBoundingSphere(ID3DXMesh *mesh, BoundingSphere *sphere);
	bool ComputeBoundingBox(ID3DXMesh *mesh, BoundingBox *box);
};