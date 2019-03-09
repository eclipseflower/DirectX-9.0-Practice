#include "CameraApp.h"

CameraApp::CameraApp(int width, int height) :InitApp(width, height)
{
	_cameraType = AIRCRAFT;
	_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


CameraApp::~CameraApp()
{
}

bool CameraApp::Setup()
{
	D3DUtil::InitMaterial(&whiteMtrl, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&blueMtrl, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLACK, 2.0f);

	DrawBasicScene(0.0f);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void CameraApp::Cleanup()
{
}

bool CameraApp::Display(float timeDelta)
{
	if (GetAsyncKeyState('W') & 0x8000f)
		Walk(4.0f * timeDelta);
	if (GetAsyncKeyState('S') & 0x8000f)
		Walk(-4.0f * timeDelta);
	if (GetAsyncKeyState('A') & 0x8000f)
		Strafe(-4.0f * timeDelta);
	if (GetAsyncKeyState('D') & 0x8000f)
		Strafe(4.0f * timeDelta);
	if (GetAsyncKeyState('R') & 0x8000f)
		Fly(4.0f * timeDelta);
	if (GetAsyncKeyState('F') & 0x8000f)
		Fly(-4.0f * timeDelta);
	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		Pitch(1.0f * timeDelta);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		Pitch(-1.0f * timeDelta);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		Yaw(1.0f * timeDelta);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		Yaw(-1.0f * timeDelta);
	if (GetAsyncKeyState('N') & 0x8000f)
		Roll(1.0f * timeDelta);
	if (GetAsyncKeyState('M') & 0x8000f)
		Roll(-1.0f * timeDelta);

	D3DXMATRIX v;
	GetViewMatrix(&v);
	device->SetTransform(D3DTS_VIEW, &v);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	device->BeginScene();
	DrawBasicScene(1.0f);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

void CameraApp::GetViewMatrix(D3DXMATRIX * v)
{
	D3DXVec3Normalize(&_look, &_look);
	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);
	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);
	float x = -D3DXVec3Dot(&_pos, &_right);
	float y = -D3DXVec3Dot(&_pos, &_up);
	float z = -D3DXVec3Dot(&_pos, &_look);

	(*v)(0, 0) = _right.x; (*v)(0, 1) = _up.x; (*v)(0, 2) = _look.x; (*v)(0, 3) = 0.0f;
	(*v)(1, 0) = _right.y; (*v)(1, 1) = _up.y; (*v)(1, 2) = _look.y; (*v)(1, 3) = 0.0f;
	(*v)(2, 0) = _right.z; (*v)(2, 1) = _up.z; (*v)(2, 2) = _look.z; (*v)(2, 3) = 0.0f;
	(*v)(3, 0) = x;        (*v)(3, 1) = y;     (*v)(3, 2) = z;       (*v)(3, 3) = 1.0f;
}

void CameraApp::GetPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

void CameraApp::SetPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void CameraApp::GetRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void CameraApp::GetUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void CameraApp::GetLook(D3DXVECTOR3* look)
{
	*look = _look;
}

void CameraApp::SetCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}

void CameraApp::Pitch(float angle)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationAxis(&mat, &_right, angle);
	D3DXVec3TransformNormal(&_look, &_look, &mat);
	D3DXVec3TransformNormal(&_up, &_up, &mat);
}

void CameraApp::Yaw(float angle)
{
	D3DXMATRIX mat;
	if (_cameraType == LANDOBJECT) 
	{
		D3DXMatrixRotationY(&mat, angle);
	}
	if (_cameraType == AIRCRAFT)
	{
		D3DXMatrixRotationAxis(&mat, &_up, angle);
	}
	D3DXVec3TransformNormal(&_look, &_look, &mat);
	D3DXVec3TransformNormal(&_right, &_right, &mat);
}

void CameraApp::Roll(float angle)
{
	if (_cameraType == AIRCRAFT) 
	{
		D3DXMATRIX mat;
		D3DXMatrixRotationAxis(&mat, &_look, angle);
		D3DXVec3TransformNormal(&_up, &_up, &mat);
		D3DXVec3TransformNormal(&_right, &_right, &mat);
	}
}

void CameraApp::Walk(float units)
{
	if (_cameraType == LANDOBJECT)
	{
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;
	}

	if (_cameraType == AIRCRAFT)
	{
		_pos += _look * units;
	}
}

void CameraApp::Strafe(float units)
{
	if (_cameraType == LANDOBJECT)
	{
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;
	}

	if (_cameraType == AIRCRAFT)
	{
		_pos += _right * units;
	}
}

void CameraApp::Fly(float units)
{
	if (_cameraType == LANDOBJECT)
	{
		_pos.y += units;
	}

	if (_cameraType == AIRCRAFT)
	{
		_pos += _up * units;
	}
}

bool CameraApp::DrawBasicScene(float scale)
{
	static IDirect3DVertexBuffer9* floor = NULL;
	static IDirect3DTexture9*      tex = NULL;
	static ID3DXMesh*              pillar = NULL;

	HRESULT hr = 0;

	if (floor == NULL && tex == NULL && pillar == NULL)
	{
		// they don't exist, create them
		device->CreateVertexBuffer(6 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_MANAGED, &floor, 0);

		Vertex* v = 0;
		floor->Lock(0, 0, (void**)&v, 0);

		v[0] = Vertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[1] = Vertex(-20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		v[3] = Vertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[4] = Vertex(20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[5] = Vertex(20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		floor->Unlock();

		D3DXCreateCylinder(device, 0.5f, 0.5f, 5.0f, 20, 20, &pillar, 0);

		D3DXCreateTextureFromFile(device, "desert.bmp", &tex);
	}
	else
	{
		//
		// Pre-Render Setup
		//
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light;
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = col * 0.4f;
		light.Diffuse = col;
		light.Specular = col * 0.6f;
		light.Direction = dir;

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);

		//
		// Render
		//

		D3DXMATRIX T, R, P, S;

		D3DXMatrixScaling(&S, scale, scale, scale);

		// used to rotate cylinders to be parallel with world's y-axis
		D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);

		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
		device->SetTransform(D3DTS_WORLD, &T);
		device->SetMaterial(&whiteMtrl);
		device->SetTexture(0, tex);
		device->SetStreamSource(0, floor, 0, sizeof(Vertex));
		device->SetFVF(Vertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// draw pillars
		device->SetMaterial(&blueMtrl);
		device->SetTexture(0, 0);
		for (int i = 0; i < 5; i++)
		{
			D3DXMatrixTranslation(&T, -5.0f, 0.0f, -15.0f + (i * 7.5f));
			P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);
			pillar->DrawSubset(0);

			D3DXMatrixTranslation(&T, 5.0f, 0.0f, -15.0f + (i * 7.5f));
			P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);
			pillar->DrawSubset(0);
		}
	}
	return true;
}