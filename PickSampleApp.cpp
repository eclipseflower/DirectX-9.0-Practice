#include "PickSampleApp.h"


PickSampleApp::PickSampleApp(int width, int height) : InitApp(width, height)
{
}

bool PickSampleApp::Setup()
{
	D3DXCreateTeapot(device, &teapot, 0);

	D3DUtil::InitMaterial(&yellowMtrl, D3DUtil::YELLOW, D3DUtil::YELLOW, D3DUtil::YELLOW, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&blueMtrl, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLACK, 2.0f);

	BYTE* v = 0;
	teapot->LockVertexBuffer(0, (void**)&v);

	D3DXComputeBoundingSphere((D3DXVECTOR3*)v, teapot->GetNumVertices(), D3DXGetFVFVertexSize(teapot->GetFVF()), &boundingSphere._center, &boundingSphere._radius);

	teapot->UnlockVertexBuffer();

	D3DXCreateSphere(device, boundingSphere._radius, 20, 20, &sphere, 0);

	D3DXVECTOR3 dir(0.707f, -0.0f, 0.707f);
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
	device->SetRenderState(D3DRS_SPECULARENABLE, false);

	D3DXVECTOR3 pos(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	device->SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void PickSampleApp::Cleanup()
{
}

bool PickSampleApp::Display(float timeDelta)
{
	static float r = 0.0f;
	static float v = 1.0f;
	static float angle = 0.0f;

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, cosf(angle) * r, sinf(angle) * r, 10.0f);

	boundingSphere._center = D3DXVECTOR3(cosf(angle)*r, sinf(angle)*r, 10.0f);

	r += v * timeDelta;

	if (r >= 8.0f)
		v = -v; // reverse direction

	if (r <= 0.0f)
		v = -v; // reverse direction

	angle += 1.0f * D3DX_PI * timeDelta;
	if (angle >= D3DX_PI * 2.0f)
		angle = 0.0f;

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();

	// Render the teapot.
	device->SetTransform(D3DTS_WORLD, &world);
	device->SetMaterial(&yellowMtrl);
	teapot->DrawSubset(0);

	// Render the bounding sphere with alpha blending so we can see 
	// through it.
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 blue = blueMtrl;
	blue.Diffuse.a = 0.25f; // 25% opacity
	device->SetMaterial(&blue);
	sphere->DrawSubset(0);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	device->EndScene();
	device->Present(0, 0, 0, 0);

	return true;
}

PickSampleApp::~PickSampleApp()
{
}

Ray PickSampleApp::CalcPickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	device->GetViewport(&vp);

	D3DXMATRIX proj;
	device->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f * x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f * y) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray._origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);
	return ray;
}

void PickSampleApp::TransformRay(Ray * ray, D3DXMATRIX * mat)
{
	D3DXVec3TransformCoord(&ray->_origin, &ray->_origin, mat);
	D3DXVec3TransformNormal(&ray->_direction, &ray->_direction, mat);
	D3DXVec3Normalize(&ray->_direction, &ray->_direction);
}

bool PickSampleApp::RaySphereIntTest(Ray * ray, BoundingSphere * sphere)
{
	D3DXVECTOR3 v = ray->_origin - sphere->_center;
	float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);

	float discriminant = (b * b) - (4.0f * c);
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);
	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;
	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;
	return false;
}

void PickSampleApp::LeftMouseDown(LPARAM lParam)
{
	Ray ray = CalcPickingRay(LOWORD(lParam), HIWORD(lParam));

	D3DXMATRIX view;
	device->GetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, 0, &view);

	TransformRay(&ray, &viewInverse);

	if (RaySphereIntTest(&ray, &boundingSphere))
		::MessageBox(0, "Hit!", "HIT", 0);
}
