#include "StencilShadowApp.h"

StencilShadowApp::StencilShadowApp(int width, int height) :InitApp(width, height), teapotPosition(0.0f, 3.0f, -7.5f)
{

}

bool StencilShadowApp::Setup()
{
	D3DXCreateTeapot(device, &teapot, NULL);
	device->CreateVertexBuffer(24 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vertexBuf, 0);
	Vertex *v;
	vertexBuf->Lock(0, 0, (void **)&v, 0);

	// floor
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	v[6] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	vertexBuf->Unlock();

	D3DUtil::InitMaterial(&floorMtrl, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&wallMtrl, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::WHITE * 0.2f, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&mirrorMtrl, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::WHITE, D3DUtil::BLACK, 2.0f);
	D3DUtil::InitMaterial(&teapotMtrl, D3DUtil::YELLOW, D3DUtil::YELLOW, D3DUtil::YELLOW, D3DUtil::BLACK, 2.0f);

	D3DXCreateTextureFromFile(device, "checker.jpg", &floorTex);
	D3DXCreateTextureFromFile(device, "brick0.jpg", &wallTex);
	D3DXCreateTextureFromFile(device, "ice.bmp", &mirrorTex);

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	D3DLIGHT9 dir;
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Ambient = D3DUtil::WHITE * 0.4f;
	dir.Diffuse = D3DUtil::WHITE;
	dir.Specular = D3DUtil::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(0.707f, -0.707f, 0.707f);
	device->SetLight(0, &dir);
	device->LightEnable(0, true);

	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXVECTOR3	pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3	target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3	up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX viewMat;
	D3DXMatrixLookAtLH(&viewMat, &pos, &target, &up);
	device->SetTransform(D3DTS_VIEW, &viewMat);

	D3DXMATRIX projMat;
	D3DXMatrixPerspectiveFovLH(&projMat, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &projMat);
	return true;
}

bool StencilShadowApp::Display(float timeDelta)
{
	static float radius = 20.0f;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		teapotPosition.x -= 3.0f * timeDelta;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		teapotPosition.x += 3.0f * timeDelta;

	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		radius -= 2.0f * timeDelta;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		radius += 2.0f * timeDelta;


	static float angle = (3.0f * D3DX_PI) / 2.0f;

	if (GetAsyncKeyState('A') & 0x8000f)
		angle -= 0.5f * timeDelta;

	if (GetAsyncKeyState('S') & 0x8000f)
		angle += 0.5f * timeDelta;

	D3DXVECTOR3 position(cosf(angle) * radius, 3.0f, sinf(angle) * radius);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMat;
	D3DXMatrixLookAtLH(&viewMat, &position, &target, &up);
	device->SetTransform(D3DTS_VIEW, &viewMat);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xff000000, 1.0f, 0L);
	device->BeginScene();
	RenderScene();
	RenderShadow();
	device->EndScene();
	device->Present(0, 0, 0, 0);

	return true;
}

void StencilShadowApp::RenderScene()
{
	device->SetMaterial(&teapotMtrl);
	device->SetTexture(0, NULL);
	D3DXMATRIX worldMat;
	D3DXMatrixTranslation(&worldMat, teapotPosition.x, teapotPosition.y, teapotPosition.z);
	device->SetTransform(D3DTS_WORLD, &worldMat);
	teapot->DrawSubset(0);

	D3DXMatrixIdentity(&worldMat);
	device->SetTransform(D3DTS_WORLD, &worldMat);

	device->SetStreamSource(0, vertexBuf, 0, sizeof(Vertex));
	device->SetFVF(Vertex::FVF);
	device->SetMaterial(&floorMtrl);
	device->SetTexture(0, floorTex);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	device->SetMaterial(&wallMtrl);
	device->SetTexture(0, wallTex);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	device->SetMaterial(&mirrorMtrl);
	device->SetTexture(0, mirrorTex);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}

void StencilShadowApp::RenderShadow()
{
	device->SetRenderState(D3DRS_STENCILENABLE, true);
	device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	device->SetRenderState(D3DRS_STENCILREF, 0x0);
	device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
	D3DXPLANE plane(0.0f, -1.0f, 0.0f, 0.0f);

	D3DXMATRIX shadowMat;
	D3DXMatrixShadow(&shadowMat, &lightDirection, &plane);

	D3DXMATRIX tranMat;
	D3DXMatrixTranslation(&tranMat, teapotPosition.x, teapotPosition.y, teapotPosition.z);

	D3DXMATRIX worldMat = tranMat * shadowMat;
	device->SetTransform(D3DTS_WORLD, &worldMat);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 shadowMtrl;
	D3DUtil::InitMaterial(&shadowMtrl, D3DUtil::BLACK, D3DUtil::BLACK, D3DUtil::BLACK, D3DUtil::BLACK, 0.0f);
	shadowMtrl.Diffuse.a = 0.5f;
	device->SetRenderState(D3DRS_ZENABLE, false);
	device->SetMaterial(&shadowMtrl);
	device->SetTexture(0, NULL);
	teapot->DrawSubset(0);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_STENCILENABLE, false);
}

void StencilShadowApp::Cleanup()
{

}