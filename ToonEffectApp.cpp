#include "ToonEffectApp.h"

ToonEffectApp::ToonEffectApp(int width, int height) : InitApp(width, height)
{
}

bool ToonEffectApp::Setup()
{
	D3DXCreateTeapot(device, &meshes[0], NULL);
	D3DXCreateSphere(device, 1.0f, 20, 20, &meshes[1], NULL);
	D3DXCreateTorus(device, 0.5f, 1.0f, 20, 20, &meshes[2], NULL);
	D3DXCreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20, &meshes[3], NULL);

	D3DXMatrixTranslation(&worldMatrices[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrices[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrices[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrices[3], 3.0f, 0.0f, 0.0f);

	meshColors[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	meshColors[1] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	meshColors[2] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	meshColors[3] = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	ID3DXBuffer *errorBuffer = NULL;
	D3DXCreateEffectFromFile(device, "tooneffect.fx", NULL, NULL, D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &toonEffect, &errorBuffer);

	if (errorBuffer)
	{
		MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		D3DUtil::Release<ID3DXBuffer*>(errorBuffer);
		return false;
	}

	toonTechHandle = toonEffect->GetTechniqueByName("Toon");
	shadeTexHandle = toonEffect->GetParameterByName(0, "ShadeTex");
	worldViewHandle = toonEffect->GetParameterByName(0, "WorldViewMatrix");
	worldViewProjHandle = toonEffect->GetParameterByName(0, "WorldViewProjMatrix");
	colorHandle = toonEffect->GetParameterByName(0, "Color");
	lightDirHandle = toonEffect->GetParameterByName(0, "LightDirection");

	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);

	D3DXVECTOR4 lightDirection(-0.57f, 0.57f, -0.57f, 0.0f);
	toonEffect->SetVector(lightDirHandle, &lightDirection);

	IDirect3DTexture9* tex = NULL;
	D3DXCreateTextureFromFile(device, "toonshade.bmp", &tex);
	toonEffect->SetTexture(shadeTexHandle, tex);
	D3DUtil::Release<IDirect3DTexture9*>(tex);

	return true;
}

void ToonEffectApp::Cleanup()
{
}

bool ToonEffectApp::Display(float timeDelta)
{
	static float angle = (3.0f * D3DX_PI) / 2.0f;
	static float height = 5.0f;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		angle -= 0.5f * timeDelta;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		angle += 0.5f * timeDelta;

	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		height += 5.0f * timeDelta;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		height -= 5.0f * timeDelta;

	D3DXVECTOR3 position(cosf(angle) * 10.0f, height, sinf(angle) * 10.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &target, &up);

	device->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	toonEffect->SetTechnique(toonTechHandle);
	UINT numPass = 0;
	toonEffect->Begin(&numPass, 0);
	D3DXMATRIX worldView;
	D3DXMATRIX worldViewProj;
	for (int i = 0; i < numPass; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			toonEffect->BeginPass(i);
			worldView = worldMatrices[j] * view;
			worldViewProj = worldMatrices[j] * view * projMatrix;

			toonEffect->SetMatrix(worldViewHandle, &worldView);
			toonEffect->SetMatrix(worldViewProjHandle, &worldViewProj);
			toonEffect->SetVector(colorHandle, &meshColors[j]);
			meshes[j]->DrawSubset(0);
			toonEffect->EndPass();
		}
	}
	toonEffect->End();
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

ToonEffectApp::~ToonEffectApp()
{
}
