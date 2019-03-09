#include "ColorTriangleApp.h"

ColorTriangleApp::ColorTriangleApp(int width, int height) :InitApp(width, height)
{

}

bool ColorTriangleApp::Setup() 
{
	device->CreateVertexBuffer(3 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &triangle, NULL);
	Vertex *v;
	triangle->Lock(0, 0, (void **)&v, 0);
	v[0] = Vertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = Vertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	v[2] = Vertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));
	triangle->Unlock();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	D3DXMATRIX testMatrix;
	MathLibMatrixPerspectiveFovLH(&testMatrix, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);
	device->SetRenderState(D3DRS_LIGHTING, false);
	return true;
}

void ColorTriangleApp::Cleanup() 
{

}

bool ColorTriangleApp::Display(float timeDelta) 
{
	D3DXMATRIX worldMat;
	device->SetFVF(Vertex::FVF);
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	device->SetStreamSource(0, triangle, 0, sizeof(Vertex));
	D3DXMatrixTranslation(&worldMat, -1.25f, 0.0f, 0.0f);
	device->SetTransform(D3DTS_WORLD, &worldMat);
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	D3DXMatrixTranslation(&worldMat, 1.25f, 0.0f, 0.0f);
	device->SetTransform(D3DTS_WORLD, &worldMat);
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}