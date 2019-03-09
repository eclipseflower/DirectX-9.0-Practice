#include "FireworkApp.h"
#include <ctime>

FireworkApp::FireworkApp(int width, int height) : PSystemApp(width,height)
{
	srand((unsigned int)time(0));
	D3DXVECTOR3 origin(0.0f, 10.0f, 50.0f);

	_origin = origin;
	_size = 0.9f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < 6000; i++)
		AddParticle();
}

bool FireworkApp::Setup()
{
	D3DUtil::InitMaterial(&whiteMtrl, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&blueMtrl, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLACK, 2.0f);
	Init(device, "flare.bmp");

	DrawBasicScene(1.0f);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4.0f, (float)cfgWidth / (float)cfgHeight, 1.0f, 5000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool FireworkApp::Display(float timeDelta)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		Walk(4.0f * timeDelta);

	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		Walk(-4.0f * timeDelta);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		Yaw(-1.0f * timeDelta);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		Yaw(1.0f * timeDelta);

	if (GetAsyncKeyState('N') & 0x8000f)
		Strafe(-4.0f * timeDelta);

	if (GetAsyncKeyState('M') & 0x8000f)
		Strafe(4.0f * timeDelta);

	if (GetAsyncKeyState('W') & 0x8000f)
		Pitch(1.0f * timeDelta);

	if (GetAsyncKeyState('S') & 0x8000f)
		Pitch(-1.0f * timeDelta);

	D3DXMATRIX V;
	GetViewMatrix(&V);
	device->SetTransform(D3DTS_VIEW, &V);

	Update(timeDelta);

	if (IsDead())
		Reset();

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	device->BeginScene();

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	device->SetTransform(D3DTS_WORLD, &I);

	DrawBasicScene(1.0f);

	device->SetTransform(D3DTS_WORLD, &I);
	Render();

	device->EndScene();
	device->Present(0, 0, 0, 0);

	return true;
}

void FireworkApp::Cleanup()
{
}


FireworkApp::~FireworkApp()
{
}

void FireworkApp::ResetParticle(Attribute * attribute)
{
	attribute->_isAlive = true;
	attribute->_position = _origin;
	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DUtil::GetRandomVector(&attribute->_velocity, &min, &max);
	D3DXVec3Normalize(&attribute->_velocity, &attribute->_velocity);
	attribute->_velocity *= 100.0f;
	attribute->_color = D3DXCOLOR(D3DUtil::GetRandomFloat(0.0f, 1.0f), D3DUtil::GetRandomFloat(0.0f, 1.0f), D3DUtil::GetRandomFloat(0.0f, 1.0f), 1.0f);
	attribute->_age = 0.0f;
	attribute->_lifeTime = 2.0f;
}

void FireworkApp::Update(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); ++i)
	{
		if (i->_isAlive)
		{
			i->_position += i->_velocity * timeDelta;
			i->_age += timeDelta;
			if (i->_age > i->_lifeTime)
				i->_isAlive = false;
		}
	}
}

void FireworkApp::PreRender()
{
	PSystemApp::PreRender();
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void FireworkApp::PostRender()
{
	PSystemApp::PostRender();
	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
