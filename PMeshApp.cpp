#include "PMeshApp.h"

PMeshApp::PMeshApp(int width, int height) :InitApp(width, height), mtrls(0), textures(0)
{
}

bool PMeshApp::Setup()
{
	D3DUtil::InitMaterial(&yellowMtrl, D3DUtil::YELLOW, D3DUtil::YELLOW, D3DUtil::YELLOW, D3DUtil::BLACK, 2.0f);
	ID3DXBuffer *adjBuf = NULL;
	ID3DXBuffer *mtrlBuf = NULL;
	DWORD numMtrls = 0;
	D3DXLoadMeshFromX("bigship1.x", D3DXMESH_MANAGED, device, &adjBuf, &mtrlBuf, NULL, &numMtrls, &srcMesh);
	if (mtrlBuf != NULL && numMtrls != 0)
	{
		D3DXMATERIAL *mtrl = (D3DXMATERIAL *)mtrlBuf->GetBufferPointer();
		for (DWORD i = 0; i < numMtrls; ++i)
		{
			mtrl[i].MatD3D.Ambient = mtrl[i].MatD3D.Diffuse;
			mtrls.push_back(mtrl[i].MatD3D);

			if (mtrl[i].pTextureFilename != NULL)
			{
				IDirect3DTexture9 *tex = NULL;
				D3DXCreateTextureFromFile(device, mtrl[i].pTextureFilename, &tex);
				textures.push_back(tex);
			}
			else
			{
				textures.push_back(NULL);
			}
		}
	}
	D3DUtil::Release<ID3DXBuffer*>(mtrlBuf);

	srcMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)adjBuf->GetBufferPointer(), (DWORD*)adjBuf->GetBufferPointer(), 0, 0);

	D3DXGeneratePMesh(srcMesh, (DWORD*)adjBuf->GetBufferPointer(), NULL, NULL, 1, D3DXMESHSIMP_FACE, &pMesh);
	D3DUtil::Release<ID3DXBuffer*>(adjBuf);
	D3DUtil::Release<ID3DXMesh*>(srcMesh);

	DWORD maxFaces = pMesh->GetMaxFaces();
	pMesh->SetNumFaces(maxFaces);

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
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
	device->SetRenderState(D3DRS_SPECULARENABLE, true);

	D3DXVECTOR3 pos(-8.0f, 4.0f, -12.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &pos, &target, &up);
	device->SetTransform(D3DTS_VIEW, &view);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void PMeshApp::Cleanup()
{
}

bool PMeshApp::Display(float timeDelta)
{
	int num = pMesh->GetNumFaces();
	if (GetAsyncKeyState('A') & 0x8000f)
	{
		pMesh->SetNumFaces(num + 1);
		if (pMesh->GetNumFaces() == num)
		{
			pMesh->SetNumFaces(num + 2);
		}
	}

	if (GetAsyncKeyState('S') & 0x8000f)
	{
		pMesh->SetNumFaces(num - 1);
	}

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	for (size_t i = 0; i < mtrls.size(); ++i)
	{
		device->SetMaterial(&mtrls[i]);
		device->SetTexture(0, textures[i]);
		pMesh->DrawSubset(i);

		device->SetMaterial(&yellowMtrl);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pMesh->DrawSubset(i);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}
