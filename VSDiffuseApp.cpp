#include "VSDiffuseApp.h"

VSDiffuseApp::VSDiffuseApp(int width, int height) : InitApp(width, height)
{
}

bool VSDiffuseApp::Setup()
{
	D3DXCreateTeapot(device, &teapot, NULL);
	ID3DXBuffer *shader = NULL;
	ID3DXBuffer *errorBuf = NULL;

	D3DXCompileShaderFromFile("diffuse.hlsl", NULL, NULL, "Main", "vs_1_1", D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shader, &errorBuf, &diffuseConstantTable);
	
	if (errorBuf)
	{
		MessageBox(0, (char*)errorBuf->GetBufferPointer(), 0, 0);
		return false;
	}

	device->CreateVertexShader((DWORD *)shader->GetBufferPointer(), &diffuseShader);
	D3DUtil::Release<ID3DXBuffer*>(shader);
	viewMatrixHandle = diffuseConstantTable->GetConstantByName(0, "ViewMatrix");
	viewProjMatrixHandle = diffuseConstantTable->GetConstantByName(0, "ViewProjMatrix");
	ambientMtrlHandle = diffuseConstantTable->GetConstantByName(0, "AmbientMtrl");
	diffuseMtrlHandle = diffuseConstantTable->GetConstantByName(0, "DiffuseMtrl");
	lightDirHandle = diffuseConstantTable->GetConstantByName(0, "LightDirection");

	D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);
	diffuseConstantTable->SetVector(device, lightDirHandle, &directionToLight);

	D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);

	diffuseConstantTable->SetVector(device, ambientMtrlHandle, &ambientMtrl);
	diffuseConstantTable->SetVector(device, diffuseMtrlHandle, &diffuseMtrl);
	diffuseConstantTable->SetDefaults(device);

	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);

	return true;
}

void VSDiffuseApp::Cleanup()
{
}

bool VSDiffuseApp::Display(float timeDelta)
{
	static float angle = (3.0f * D3DX_PI) / 2.0f;
	static float height = 3.0f;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		angle -= 0.5f * timeDelta;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		angle += 0.5f * timeDelta;

	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		height += 5.0f * timeDelta;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		height -= 5.0f * timeDelta;

	D3DXVECTOR3 position(cosf(angle) * 7.0f, height, sinf(angle) * 7.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	diffuseConstantTable->SetMatrix(device, viewMatrixHandle, &V);

	D3DXMATRIX ViewProj = V * proj;
	diffuseConstantTable->SetMatrix(device, viewProjMatrixHandle, &ViewProj);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	device->SetVertexShader(diffuseShader);
	teapot->DrawSubset(0);
	device->EndScene();
	device->Present(0, 0, 0, 0);

	return true;
}

VSDiffuseApp::~VSDiffuseApp()
{
}
