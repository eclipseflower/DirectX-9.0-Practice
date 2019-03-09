#pragma once
#include "InitApp.h"

struct Vertex
{
	float _x, _y, _z;
	D3DCOLOR _color;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	Vertex() {}
	Vertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x;
		_y = y;
		_z = z;
		_color = c;
	}
};

class ColorTriangleApp : public InitApp
{
public:
	IDirect3DVertexBuffer9 *triangle = NULL;
	ColorTriangleApp(int with, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
};