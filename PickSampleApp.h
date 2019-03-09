#pragma once
#include "InitApp.h"

struct Ray
{
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};

struct BoundingSphere
{
	D3DXVECTOR3 _center;
	float _radius;
};

class PickSampleApp : public InitApp
{
public:
	PickSampleApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	~PickSampleApp();
	Ray CalcPickingRay(int x, int y);
	void TransformRay(Ray *ray, D3DXMATRIX *mat);
	bool RaySphereIntTest(Ray *ray, BoundingSphere *sphere);
	void LeftMouseDown(LPARAM lParam);

	ID3DXMesh *teapot = NULL;
	ID3DXMesh *sphere = NULL;
	BoundingSphere boundingSphere;
	D3DMATERIAL9 yellowMtrl;
	D3DMATERIAL9 blueMtrl;
};

