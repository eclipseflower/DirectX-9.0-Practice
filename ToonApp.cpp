#include "ToonApp.h"

ToonApp::ToonApp(int width, int height) : InitApp(width, height)
{
}

bool ToonApp::Setup()
{
	ID3DXBuffer *adjBuffer[4] = { NULL, NULL, NULL, NULL };
	D3DXCreateTeapot(device, &meshes[0], &adjBuffer[0]);
	D3DXCreateSphere(device, 1.0f, 20, 20, &meshes[1], &adjBuffer[1]);
	D3DXCreateTorus(device, 0.5f, 1.0f, 20, 20, &meshes[2], &adjBuffer[2]);
	D3DXCreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20, &meshes[3], &adjBuffer[3]);

	D3DXMatrixTranslation(&worldMatrices[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrices[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrices[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&worldMatrices[3], 3.0f, 0.0f, 0.0f);

	meshColors[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	meshColors[1] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	meshColors[2] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	meshColors[3] = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	meshOutlines[0] = new SilhouetteEdges(device, meshes[0], adjBuffer[0]);
	meshOutlines[1] = new SilhouetteEdges(device, meshes[1], adjBuffer[1]);
	meshOutlines[2] = new SilhouetteEdges(device, meshes[2], adjBuffer[2]);
	meshOutlines[3] = new SilhouetteEdges(device, meshes[3], adjBuffer[3]);

	for (int i = 0; i < 4; ++i)
	{
		GenEdgeVertices(meshOutlines[i], meshes[i], adjBuffer[i]);
		GenEdgeIndices(meshOutlines[i], meshes[i]);
		CreateVertexDeclaration(meshOutlines[i]);
	}

	ID3DXBuffer *toonCompileCode = NULL;
	ID3DXBuffer *toonErrorBuffer = NULL;

	D3DXCompileShaderFromFile("toon.hlsl", 0, 0, "Main", "vs_1_1", D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &toonCompileCode, &toonErrorBuffer, &toonConstTable);

	if (toonErrorBuffer)
	{
		MessageBox(0, (char*)toonErrorBuffer->GetBufferPointer(), 0, 0);
		return false;
	}

	device->CreateVertexShader((DWORD *)toonCompileCode->GetBufferPointer(), &toonShader);

	ID3DXBuffer *outlineCompileCode = NULL;
	ID3DXBuffer *outlineErrorBuffer = NULL;

	D3DXCompileShaderFromFile("outline.hlsl", 0, 0, "Main", "vs_1_1", D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &outlineCompileCode, &outlineErrorBuffer, &outlineConstTable);

	if (outlineErrorBuffer)
	{
		MessageBox(0, (char*)outlineErrorBuffer->GetBufferPointer(), 0, 0);
		return false;
	}

	device->CreateVertexShader((DWORD *)outlineCompileCode->GetBufferPointer(), &outlineShader);

	D3DXCreateTextureFromFile(device, "toonshade.bmp", &shadeTex);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	toonWorldViewHandle = toonConstTable->GetConstantByName(0, "WorldViewMatrix");
	toonWorldViewProjHandle = toonConstTable->GetConstantByName(0, "WorldViewProjMatrix");
	toonColorHandle = toonConstTable->GetConstantByName(0, "Color");
	toonLightDirHandle = toonConstTable->GetConstantByName(0, "LightDirection");

	outlineWorldViewHandle = outlineConstTable->GetConstantByName(0, "WorldViewMatrix");
	outlineProjHandle = outlineConstTable->GetConstantByName(0, "ProjMatrix");

	D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);

	toonConstTable->SetVector(device, toonLightDirHandle, &directionToLight);

	toonConstTable->SetDefaults(device);
	outlineConstTable->SetDefaults(device);

	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);

	return true;
}

void ToonApp::Cleanup()
{
}

bool ToonApp::Display(float timeDelta)
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

	D3DXVECTOR3 position(cosf(angle) * 7.0f, height, sinf(angle) * 7.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &target, &up);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	device->SetVertexShader(toonShader);
	device->SetTexture(0, shadeTex);

	D3DXMATRIX worldView;
	D3DXMATRIX worldViewProj;
	for (int i = 0; i < 4; ++i)
	{
		worldView = worldMatrices[i] * view;
		worldViewProj = worldMatrices[i] * view * projMatrix;
		toonConstTable->SetMatrix(device, toonWorldViewHandle, &worldView);
		toonConstTable->SetMatrix(device, toonWorldViewProjHandle, &worldViewProj);
		toonConstTable->SetVector(device, toonColorHandle, &meshColors[i]);
		meshes[i]->DrawSubset(0);
	}

	device->SetVertexShader(outlineShader);
	device->SetTexture(0, NULL);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int i = 0; i < 4; ++i)
	{
		worldView = worldMatrices[i] * view;
		outlineConstTable->SetMatrix(device, outlineWorldViewHandle, &worldView);
		outlineConstTable->SetMatrix(device, outlineProjHandle, &projMatrix);
		Render(meshOutlines[i]);
	}

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

void ToonApp::GenEdgeVertices(SilhouetteEdges * outline, ID3DXMesh * mesh, ID3DXBuffer * adjBuffer)
{
	outline->_numVerts = mesh->GetNumFaces() * 3 * 4;
	device->CreateVertexBuffer(outline->_numVerts * sizeof(EdgeVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &outline->_vb, NULL);

	MeshVertex *vertices = NULL;
	mesh->LockVertexBuffer(0, (void **)&vertices);
	WORD *indices = NULL;
	mesh->LockIndexBuffer(0, (void **)&indices);

	EdgeVertex *edgeVertices = NULL;
	outline->_vb->Lock(0, 0, (void **)&edgeVertices, 0);

	for (int i = 0; i < mesh->GetNumFaces(); ++i)
	{
		D3DXVECTOR3 currentFaceNormal;
		D3DXVECTOR3 adjFaceNormals[3];
		GetFaceNormals(mesh, adjBuffer, &currentFaceNormal, adjFaceNormals, i);

		WORD index0 = indices[i * 3];
		WORD index1 = indices[i * 3 + 1];
		WORD index2 = indices[i * 3 + 2];

		MeshVertex v0 = vertices[index0];
		MeshVertex v1 = vertices[index1];
		MeshVertex v2 = vertices[index2];

		EdgeVertex A0, B0, C0, D0;
		A0.position = v0.position;
		A0.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A0.faceNormal1 = currentFaceNormal;
		A0.faceNormal2 = adjFaceNormals[0];

		B0.position = v1.position;
		B0.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B0.faceNormal1 = currentFaceNormal;
		B0.faceNormal2 = adjFaceNormals[0];

		C0 = A0;
		C0.normal = v0.normal;

		D0 = B0;
		D0.normal = v1.normal;

		*edgeVertices = A0; ++edgeVertices;
		*edgeVertices = B0; ++edgeVertices;
		*edgeVertices = C0; ++edgeVertices;
		*edgeVertices = D0; ++edgeVertices;

		EdgeVertex A1, B1, C1, D1;
		A1.position = v1.position;
		A1.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A1.faceNormal1 = currentFaceNormal;
		A1.faceNormal2 = adjFaceNormals[1];

		B1.position = v2.position;
		B1.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B1.faceNormal1 = currentFaceNormal;
		B1.faceNormal2 = adjFaceNormals[1];

		C1 = A1;
		C1.normal = v1.normal;

		D1 = B1;
		D1.normal = v2.normal;

		*edgeVertices = A1; ++edgeVertices;
		*edgeVertices = B1; ++edgeVertices;
		*edgeVertices = C1; ++edgeVertices;
		*edgeVertices = D1; ++edgeVertices;

		EdgeVertex A2, B2, C2, D2;

		A2.position = v0.position;
		A2.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A2.faceNormal1 = currentFaceNormal;
		A2.faceNormal2 = adjFaceNormals[2];

		B2.position = v2.position;
		B2.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B2.faceNormal1 = currentFaceNormal;
		B2.faceNormal2 = adjFaceNormals[2];

		C2 = A2;
		C2.normal = v0.normal;

		D2 = B2;
		D2.normal = v2.normal;

		*edgeVertices = A2; ++edgeVertices;
		*edgeVertices = B2; ++edgeVertices;
		*edgeVertices = C2; ++edgeVertices;
		*edgeVertices = D2; ++edgeVertices;
	}

	outline->_vb->Unlock();
	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}

void ToonApp::GetFaceNormals(ID3DXMesh * mesh, ID3DXBuffer * adjBuffer, D3DXVECTOR3 * currentFaceNormal, D3DXVECTOR3 adjFaceNormals[3], DWORD faceIndex)
{
	MeshVertex *vertices = NULL;
	mesh->LockVertexBuffer(0, (void **)&vertices);
	WORD *indices = NULL;
	mesh->LockIndexBuffer(0, (void **)&indices);

	DWORD *adj = (DWORD *)adjBuffer->GetBufferPointer();

	GetFaceNormal(mesh, faceIndex, currentFaceNormal);

	DWORD faceIndex0 = adj[faceIndex * 3];
	DWORD faceIndex1 = adj[faceIndex * 3 + 1];
	DWORD faceIndex2 = adj[faceIndex * 3 + 2];

	D3DXVECTOR3 faceNormal0, faceNormal1, faceNormal2;

	if (faceIndex0 != USHRT_MAX)
	{
		WORD i0 = indices[faceIndex0 * 3];
		WORD i1 = indices[faceIndex0 * 3 + 1];
		WORD i2 = indices[faceIndex0 * 3 + 2];

		D3DXVECTOR3 v0 = vertices[i0].position;
		D3DXVECTOR3 v1 = vertices[i1].position;
		D3DXVECTOR3 v2 = vertices[i2].position;
		
		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;

		D3DXVec3Cross(&faceNormal0, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal0, &faceNormal0);
	}
	else
	{
		faceNormal0 = -*currentFaceNormal;
	}

	if (faceIndex1 != USHRT_MAX)
	{
		WORD i0 = indices[faceIndex1 * 3];
		WORD i1 = indices[faceIndex1 * 3 + 1];
		WORD i2 = indices[faceIndex1 * 3 + 2];

		D3DXVECTOR3 v0 = vertices[i0].position;
		D3DXVECTOR3 v1 = vertices[i1].position;
		D3DXVECTOR3 v2 = vertices[i2].position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal1, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal1, &faceNormal1);
	}
	else
	{
		faceNormal1 = -(*currentFaceNormal);
	}

	if (faceIndex2 != USHRT_MAX)
	{
		WORD i0 = indices[faceIndex2 * 3];
		WORD i1 = indices[faceIndex2 * 3 + 1];
		WORD i2 = indices[faceIndex2 * 3 + 2];

		D3DXVECTOR3 v0 = vertices[i0].position;
		D3DXVECTOR3 v1 = vertices[i1].position;
		D3DXVECTOR3 v2 = vertices[i2].position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal2, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal2, &faceNormal2);
	}
	else
	{
		faceNormal2 = -(*currentFaceNormal);
	}

	adjFaceNormals[0] = faceNormal0;
	adjFaceNormals[1] = faceNormal1;
	adjFaceNormals[2] = faceNormal2;

	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}

void ToonApp::GetFaceNormal(ID3DXMesh * mesh, DWORD faceIndex, D3DXVECTOR3 * faceNormal)
{
	MeshVertex *vertices = NULL;
	mesh->LockVertexBuffer(0, (void **)&vertices);
	WORD *indices = NULL;
	mesh->LockIndexBuffer(0, (void **)&indices);

	WORD index0 = indices[faceIndex * 3];
	WORD index1 = indices[faceIndex * 3 + 1];
	WORD index2 = indices[faceIndex * 3 + 2];

	D3DXVECTOR3 v0 = vertices[index0].position;
	D3DXVECTOR3 v1 = vertices[index1].position;
	D3DXVECTOR3 v2 = vertices[index2].position;

	D3DXVECTOR3 edge0, edge1;
	edge0 = v1 - v0;
	edge1 = v2 - v0;
	D3DXVec3Cross(faceNormal, &edge0, &edge1);
	D3DXVec3Normalize(faceNormal, faceNormal);

	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}

void ToonApp::GenEdgeIndices(SilhouetteEdges * outline, ID3DXMesh * mesh)
{
	DWORD numEdges = mesh->GetNumFaces() * 3;
	outline->_numFaces = numEdges * 2;
	device->CreateIndexBuffer(numEdges * 6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &outline->_ib, NULL);

	WORD *indices = NULL;
	outline->_ib->Lock(0, 0, (void **)&indices, 0);

	for (UINT i = 0; i < numEdges; ++i)
	{
		indices[i * 6] = i * 4;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 1;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 2;
	}

	outline->_ib->Unlock();
}

bool ToonApp::CreateVertexDeclaration(SilhouetteEdges * outline)
{
	D3DVERTEXELEMENT9 decl[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 1},
		{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 2},
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(decl, &outline->_decl);
	return true;
}

void ToonApp::Render(SilhouetteEdges * outline)
{
	device->SetVertexDeclaration(outline->_decl);
	device->SetStreamSource(0, outline->_vb, 0, sizeof(EdgeVertex));
	device->SetIndices(outline->_ib);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, outline->_numVerts, 0, outline->_numFaces);
}

ToonApp::~ToonApp()
{
}
