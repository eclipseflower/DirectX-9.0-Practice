#include "TexQuadApp.h"

TexQuadApp::TexQuadApp(int width, int height) :InitApp(width, height)
{

}

bool TexQuadApp::Setup()
{
	device->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &quad, 0);
	Vertex *v;
	quad->Lock(0, 0, (void **)&v, 0);
	v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f);
	v[1] = Vertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f);
	v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f);
	v[4] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 3.0f);
	quad->Unlock();

	D3DXCreateTextureFromFile(device, "dx5_logo.bmp", &tex);
	device->SetTexture(0, tex);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	device->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}

bool TexQuadApp::Display(float timeDelta)
{
	if (GetAsyncKeyState('W') & 0x8000f) 
	{
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}
	else if (GetAsyncKeyState('B') & 0x8000f)
	{
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);
	}
	else if (GetAsyncKeyState('C') & 0x8000f)
	{
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}
	else if (GetAsyncKeyState('M') & 0x8000f)
	{
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	}

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	device->SetStreamSource(0, quad, 0, sizeof(Vertex));
	device->SetFVF(Vertex::FVF);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

void TexQuadApp::Cleanup()
{

}