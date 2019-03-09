#pragma once
#include "InitApp.h"

struct EdgeVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 faceNormal1;
	D3DXVECTOR3 faceNormal2;
};

struct MeshVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
};

struct SilhouetteEdges
{
	SilhouetteEdges(IDirect3DDevice9* device, ID3DXMesh* mesh, ID3DXBuffer* adjBuffer)
	{
		_device = device;
		_vb = NULL;
		_ib = NULL;
		_decl = NULL;
		_numVerts = NULL;
		_numFaces = NULL;
	}

	IDirect3DDevice9*            _device;
	IDirect3DVertexBuffer9*      _vb;
	IDirect3DIndexBuffer9*       _ib;
	IDirect3DVertexDeclaration9* _decl;

	UINT _numVerts;
	UINT _numFaces;
};

class ToonApp : public InitApp
{
public:
	ToonApp(int width, int height);
	bool Setup() override;
	void Cleanup() override;
	bool Display(float timeDelta) override;
	void GenEdgeVertices(SilhouetteEdges *outline, ID3DXMesh* mesh, ID3DXBuffer* adjBuffer);
	void GetFaceNormals(ID3DXMesh* mesh, ID3DXBuffer* adjBuffer, D3DXVECTOR3* currentFaceNormal, D3DXVECTOR3  adjFaceNormals[3], DWORD faceIndex);
	void GetFaceNormal(ID3DXMesh* mesh, DWORD faceIndex, D3DXVECTOR3* faceNormal);
	void GenEdgeIndices(SilhouetteEdges *outline, ID3DXMesh* mesh);
	bool CreateVertexDeclaration(SilhouetteEdges * outline);
	void Render(SilhouetteEdges * outline);
	~ToonApp();

	ID3DXMesh*  meshes[4] = { NULL, NULL, NULL, NULL };
	D3DXMATRIX  worldMatrices[4];
	D3DXVECTOR4 meshColors[4];
	SilhouetteEdges* meshOutlines[4] = { NULL, NULL, NULL, NULL };
	IDirect3DVertexShader9* toonShader = NULL;
	ID3DXConstantTable *toonConstTable = NULL;
	IDirect3DVertexShader9* outlineShader = NULL;
	ID3DXConstantTable *outlineConstTable = NULL;
	IDirect3DTexture9* shadeTex = NULL;
	D3DXMATRIX projMatrix;

	D3DXHANDLE toonWorldViewHandle = 0;
	D3DXHANDLE toonWorldViewProjHandle = 0;
	D3DXHANDLE toonColorHandle = 0;
	D3DXHANDLE toonLightDirHandle = 0;
	D3DXHANDLE outlineWorldViewHandle = 0;
	D3DXHANDLE outlineProjHandle = 0;
};

