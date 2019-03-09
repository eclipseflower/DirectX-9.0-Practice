#include "LightTexEffectApp.h"

LightTexEffectApp::LightTexEffectApp(int width, int height) : InitApp(width, height)
{
}

bool LightTexEffectApp::Setup()
{
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls = 0;

	D3DXLoadMeshFromX("mountain.x", D3DXMESH_MANAGED, device, 0, &mtrlBuffer, 0, &numMtrls, &mesh);

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrl = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++)
		{
			mtrl[i].MatD3D.Ambient = mtrl[i].MatD3D.Diffuse;
			mtrls.push_back(mtrl[i].MatD3D);

			if (mtrl[i].pTextureFilename != NULL)
			{
				IDirect3DTexture9* tex = NULL;
				D3DXCreateTextureFromFile(device, mtrl[i].pTextureFilename, &tex);
				textures.push_back(tex);
			}
			else
			{
				textures.push_back(NULL);
			}
		}
	}
	D3DUtil::Release<ID3DXBuffer*>(mtrlBuffer);

	ID3DXBuffer *errorBuffer = NULL;
	D3DXCreateEffectFromFile(device, "light_tex.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL, &lightTexEffect, &errorBuffer);

	if (errorBuffer)
	{
		MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		D3DUtil::Release<ID3DXBuffer*>(errorBuffer);
		return false;
	}

	worldMatrixHandle = lightTexEffect->GetParameterByName(0, "WorldMatrix");
	viewMatrixHandle = lightTexEffect->GetParameterByName(0, "ViewMatrix");
	projMatrixHandle = lightTexEffect->GetParameterByName(0, "ProjMatrix");
	texHandle = lightTexEffect->GetParameterByName(0, "Tex");
	lightTexTechHandle = lightTexEffect->GetTechniqueByName("LightAndTexture");

	D3DXMATRIX W, P;
	D3DXMatrixIdentity(&W);
	lightTexEffect->SetMatrix(worldMatrixHandle, &W);
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	lightTexEffect->SetMatrix(projMatrixHandle, &P);

	IDirect3DTexture9* tex = NULL;
	D3DXCreateTextureFromFile(device, "Terrain_3x_diffcol.jpg", &tex);
	lightTexEffect->SetTexture(texHandle, tex);

	D3DUtil::Release<IDirect3DTexture9*>(tex);

	return true;
}

void LightTexEffectApp::Cleanup()
{
}

bool LightTexEffectApp::Display(float timeDelta)
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
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	lightTexEffect->SetMatrix(viewMatrixHandle, &V);
	device->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	lightTexEffect->SetTechnique(lightTexTechHandle);
	UINT numPass = 0;
	lightTexEffect->Begin(&numPass, 0);
	for (int i = 0; i < numPass; ++i)
	{
		lightTexEffect->BeginPass(i);
		for (int j = 0; j < mtrls.size(); ++j)
			mesh->DrawSubset(j);
		lightTexEffect->EndPass();
	}
	lightTexEffect->End();
	device->EndScene();
	device->Present(0, 0, 0, 0);

	return true;
}

LightTexEffectApp::~LightTexEffectApp()
{
}
