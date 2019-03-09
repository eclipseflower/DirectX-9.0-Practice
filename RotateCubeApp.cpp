#include "RotateCubeApp.h"

RotateCubeApp::RotateCubeApp(int width, int height):InitApp(width, height)
{

}

bool RotateCubeApp::Setup() 
{
	device->CreateVertexBuffer(8 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vertexBuf, NULL);
	device->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuf, NULL);

	Vertex *vertex;
	vertexBuf->Lock(0, 0, (void **)&vertex, 0);
	vertex[0] = Vertex(1.0f,	1.0f,	1.0f);
	vertex[1] = Vertex(1.0f,	1.0f,	-1.0f);
	vertex[2] = Vertex(1.0f,	-1.0f,	1.0f);
	vertex[3] = Vertex(1.0f,	-1.0f,	-1.0f);
	vertex[4] = Vertex(-1.0f,	1.0f,	1.0f);
	vertex[5] = Vertex(-1.0f,	1.0f,	-1.0f);
	vertex[6] = Vertex(-1.0f,	-1.0f,	1.0f);
	vertex[7] = Vertex(-1.0f,	-1.0f,	-1.0f);
	vertexBuf->Unlock();

	WORD *index;
	indexBuf->Lock(0, 0, (void **)&index, 0);
	// z = 1 front
	index[0] = 0; index[1] = 4; index[2] = 2;
	index[3] = 4; index[4] = 6; index[5] = 2;
	// z = -1 back
	index[6] = 1; index[7] = 3; index[8] = 5;
	index[9] = 3; index[10] = 7; index[11] = 5;
	// x = -1 left
	index[12] = 4; index[13] = 5; index[14] = 6;
	index[15] = 5; index[16] = 7; index[17] = 6;
	// x = 1 right
	index[18] = 0; index[19] = 3; index[20] = 1;
	index[21] = 0; index[22] = 2; index[23] = 3;
	// y = 1 top
	index[24] = 0; index[25] = 1; index[26] = 5;
	index[27] = 0; index[28] = 5; index[29] = 4;
	// y = -1 bottom
	index[30] = 2; index[31] = 6; index[32] = 3;
	index[33] = 3; index[34] = 6; index[35] = 7;
	indexBuf->Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &position, &target, &up);
	D3DXMATRIX testMatrix;
	MathLibMatrixLookAtLH(&testMatrix, &position, &target, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMATRIX projMatrix;
	D3DXMatrixPerspectiveFovLH(&projMatrix, 0.5f * D3DX_PI, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &projMatrix);

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}

bool RotateCubeApp::Display(float timeDelta) 
{
	D3DXMATRIX rxMat, ryMat;
	D3DXMatrixRotationX(&rxMat, 0.25f * D3DX_PI);
	D3DXMATRIX testMatrix;
	MathLibMatrixRotationX(&testMatrix, 0.25f * D3DX_PI);
	static float angle = 0.0f;
	D3DXMatrixRotationY(&ryMat, angle);
	angle += timeDelta;
	if (angle >= 2.0f * D3DX_PI)
	{
		angle = 0.0f;
	}
	D3DXMATRIX rMat = rxMat * ryMat;
	device->SetTransform(D3DTS_WORLD, &rMat);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	device->SetStreamSource(0, vertexBuf, 0, sizeof(Vertex));
	device->SetIndices(indexBuf);
	device->SetFVF(Vertex::FVF);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

void RotateCubeApp::Cleanup() 
{
	D3DUtil::Release<IDirect3DVertexBuffer9*>(vertexBuf);
	D3DUtil::Release<IDirect3DIndexBuffer9*>(indexBuf);
	InitApp::Cleanup();
}