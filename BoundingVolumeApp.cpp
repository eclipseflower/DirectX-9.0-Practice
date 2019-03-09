#include "BoundingVolumeApp.h"

BoundingVolumeApp::BoundingVolumeApp(int width, int height) :InitApp(width, height), mtrls(0), textures(0)
{
}

bool BoundingVolumeApp::Setup()
{
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

	srcMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)adjBuf->GetBufferPointer(), 0, 0, 0);
	D3DUtil::Release<ID3DXBuffer*>(adjBuf);

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	BoundingSphere sphere;
	ComputeBoundingSphere(srcMesh, &sphere);
	D3DXCreateSphere(device, sphere._radius, 20, 20, &sphereMesh, NULL);

	BoundingBox box;
	ComputeBoundingBox(srcMesh, &box);
	D3DXCreateBox(device, box._max.x - box._min.x, box._max.y - box._min.y, box._max.z - box._min.z, &boxMesh, NULL);

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

	D3DXVECTOR3 pos(4.0f, 12.0f, -20.0f);
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

void BoundingVolumeApp::Cleanup()
{
}

bool BoundingVolumeApp::Display(float timeDelta)
{
	if (GetAsyncKeyState('W') & 0x8000f)
	{
		sphereMode = !sphereMode;
	}

	D3DXMATRIX ry;
	static float angle = 0.0f;
	D3DXMatrixRotationY(&ry, angle);
	angle += timeDelta;
	if (angle >= D3DX_PI * 2.0f)
	{
		angle = 0.0f;
	}
	device->SetTransform(D3DTS_WORLD, &ry);
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	for (size_t i = 0; i < mtrls.size(); ++i)
	{
		device->SetMaterial(&mtrls[i]);
		device->SetTexture(0, textures[i]);
		srcMesh->DrawSubset(i);
	}

	D3DUtil::InitMaterial(&blueMtrl, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLUE, D3DUtil::BLACK, 2.0f);
	blueMtrl.Diffuse.a = 0.10f;
	device->SetMaterial(&blueMtrl);
	device->SetTexture(0, NULL);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if(sphereMode)
	{
		sphereMesh->DrawSubset(0);
	}
	else
	{
		boxMesh->DrawSubset(0);
	}
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

bool BoundingVolumeApp::ComputeBoundingSphere(ID3DXMesh * mesh, BoundingSphere * sphere)
{
	BYTE *v;
	mesh->LockVertexBuffer(0, (void **)&v);
	D3DXComputeBoundingSphere((D3DXVECTOR3 *)v, mesh->GetNumVertices(), D3DXGetFVFVertexSize(mesh->GetFVF()), &sphere->_center, &sphere->_radius);
	mesh->UnlockVertexBuffer();
	return true;
}

bool BoundingVolumeApp::ComputeBoundingBox(ID3DXMesh * mesh, BoundingBox * box)
{
	BYTE *v;
	mesh->LockVertexBuffer(0, (void **)&v);
	D3DXComputeBoundingBox((D3DXVECTOR3 *)v, mesh->GetNumVertices(), D3DXGetFVFVertexSize(mesh->GetFVF()), &box->_min, &box->_max);
	mesh->UnlockVertexBuffer();
	return true;
}
