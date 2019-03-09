#include "MtrlAlphaApp.h"

MtrlAlphaApp::MtrlAlphaApp(int width, int height) :InitApp(width, height)
{

}

bool MtrlAlphaApp::Setup()
{
	teapotMtrl.Ambient = D3DUtil::RED;
	teapotMtrl.Diffuse = D3DUtil::RED;
	teapotMtrl.Specular = D3DUtil::RED;
	teapotMtrl.Emissive = D3DUtil::BLACK;
	teapotMtrl.Power = 2.0f;
	teapotMtrl.Diffuse.a = 0.5f;

	bkMtrl.Ambient = D3DUtil::WHITE;
	bkMtrl.Diffuse = D3DUtil::WHITE;
	bkMtrl.Specular = D3DUtil::WHITE;
	bkMtrl.Emissive = D3DUtil::BLACK;
	bkMtrl.Power = 2.0f;

	D3DXCreateTeapot(device, &teapot, NULL);
	device->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &bkQuad, NULL);
	Vertex *v;
	bkQuad->Lock(0, 0, (void **)&v, 0);
	v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	bkQuad->Unlock();

	D3DLIGHT9 dir;
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Ambient = D3DUtil::WHITE;
	dir.Diffuse = D3DUtil::WHITE;
	dir.Specular = D3DUtil::WHITE;
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);
	device->SetLight(0, &dir);
	device->LightEnable(0, true);

	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXCreateTextureFromFile(device, "crate.jpg", &bkTex);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DXVECTOR3 pos(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &pos, &target, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void MtrlAlphaApp::Cleanup()
{

}

bool MtrlAlphaApp::Display(float timeDelta)
{
	if (GetAsyncKeyState('A') & 0x8000f)
		teapotMtrl.Diffuse.a += 0.01f;
	if (GetAsyncKeyState('S') & 0x8000f)
		teapotMtrl.Diffuse.a -= 0.01f;

	if (teapotMtrl.Diffuse.a > 1.0f)
		teapotMtrl.Diffuse.a = 1.0f;
	if (teapotMtrl.Diffuse.a < 0.0f)
		teapotMtrl.Diffuse.a = 0.0f;
	
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	D3DXMATRIX worldMatrix;
	D3DXMatrixIdentity(&worldMatrix);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);
	device->SetFVF(Vertex::FVF);
	device->SetStreamSource(0, bkQuad, 0, sizeof(Vertex));
	device->SetMaterial(&bkMtrl);
	device->SetTexture(0, bkTex);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DXMatrixScaling(&worldMatrix, 1.5f, 1.5f, 1.5f);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);
	device->SetMaterial(&teapotMtrl);
	device->SetTexture(0, NULL);
	teapot->DrawSubset(0);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	device->EndScene();
	device->Present(0,0,0,0);
	return true;
}