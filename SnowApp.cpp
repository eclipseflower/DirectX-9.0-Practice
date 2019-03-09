#include "SnowApp.h"
#include <ctime>

SnowApp::SnowApp(int width, int height) : PSystemApp(width,height)
{
	srand((unsigned int)time(0));
	_boundingBox._min = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
	_boundingBox._max = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
	_size = 0.25f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;
	for (int i = 0; i < 5000; ++i)
	{
		AddParticle();
	}
}

bool SnowApp::Setup()
{
	D3DUtil::InitMaterial(&whiteMtrl, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&blueMtrl, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLACK, 2.0f);
	Init(device, "snowflake.dds");
	DrawBasicScene(1.0f);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4.0f, (float)cfgWidth / (float)cfgHeight, 1.0f, 5000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}

bool SnowApp::Display(float timeDelta)
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

void SnowApp::Cleanup()
{
}



void SnowApp::ResetParticle(Attribute * attribute)
{
	attribute->_isAlive = true;
	D3DUtil::GetRandomVector(&attribute->_position, &_boundingBox._min, &_boundingBox._max);
	attribute->_position.y = _boundingBox._max.y;
	attribute->_velocity.x = D3DUtil::GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->_velocity.y = D3DUtil::GetRandomFloat(0.0f, 1.0f) * -10.f;
	attribute->_velocity.z = 0.0f;
	attribute->_color = D3DUtil::WHITE;
}

void SnowApp::Update(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); ++i)
	{
		i->_position += i->_velocity * timeDelta;
		if (_boundingBox.IsPointInside(i->_position) == false)
		{
			ResetParticle(&(*i));
		}
	}
}

SnowApp::~SnowApp()
{
}
