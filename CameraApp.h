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

class CameraApp: public InitApp
{
public:
	D3DMATERIAL9 whiteMtrl;
	D3DMATERIAL9 blueMtrl;
	enum CameraType { LANDOBJECT, AIRCRAFT};
	CameraApp(int width, int height);
	~CameraApp();

	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;

	void Strafe(float units);
	void Fly(float units);
	void Walk(float units);
	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);

	void GetViewMatrix(D3DXMATRIX *v);
	void SetCameraType(CameraType type);
	void GetPosition(D3DXVECTOR3 *pos);
	void SetPosition(D3DXVECTOR3 *pos);
	void GetRight(D3DXVECTOR3 *right);
	void GetUp(D3DXVECTOR3 *up);
	void GetLook(D3DXVECTOR3 *look);

private:
	CameraType _cameraType;
	D3DXVECTOR3 _right;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;

protected:
	bool DrawBasicScene(float scale);
};

