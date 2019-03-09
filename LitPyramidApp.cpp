#include "LitPyramidApp.h"

LitPyramidApp::LitPyramidApp(int width, int height) :InitApp(width, height)
{

}

bool LitPyramidApp::Setup()
{
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->CreateVertexBuffer(12 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &pyramid, NULL);

	Vertex *v;
	pyramid->Lock(0, 0, (void **)&v, 0);
	// front face
	v[0] = Vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	v[1] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
	v[2] = Vertex(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

	// left face
	v[3] = Vertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
	v[4] = Vertex(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
	v[5] = Vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

	// right face
	v[6] = Vertex(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
	v[7] = Vertex(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
	v[8] = Vertex(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

	// back face
	v[9] = Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	v[10] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
	v[11] = Vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	pyramid->Unlock();

	D3DMATERIAL9 mtrl;
	mtrl.Ambient = D3DUtil::WHITE;
	mtrl.Diffuse = D3DUtil::WHITE;
	mtrl.Specular = D3DUtil::WHITE;
	mtrl.Emissive = D3DUtil::BLACK;
	mtrl.Power = 5.0f;
	device->SetMaterial(&mtrl);

	D3DLIGHT9 dir;
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Ambient = D3DUtil::WHITE * 0.6f;
	dir.Diffuse = D3DUtil::WHITE;
	dir.Specular = D3DUtil::WHITE * 0.3f;
	dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	device->SetLight(0, &dir);
	device->LightEnable(0, true);

	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXVECTOR3 position(0.0f, 1.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &position, &target, &up);
	D3DXMATRIX testMatrix;
	MathLibMatrixLookAtLH(&testMatrix, &position, &target, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	MathLibMatrixPerspectiveFovLH(&testMatrix, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}

void LitPyramidApp::Cleanup()
{

}

bool LitPyramidApp::Display(float timeDelta)
{
	D3DXMATRIX yRot;
	static float angle = 0.0f;
	D3DXMatrixRotationY(&yRot, angle);
	D3DXMATRIX testMatrix;
	MathLibMatrixRotationY(&testMatrix, angle);
	device->SetTransform(D3DTS_WORLD, &yRot);
	angle += timeDelta;
	if (angle >= D3DX_PI * 2.0f) 
	{
		angle = 0.0f;
	}
	device->SetFVF(Vertex::FVF);
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	device->BeginScene();
	device->SetStreamSource(0, pyramid, 0, sizeof(Vertex));
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}