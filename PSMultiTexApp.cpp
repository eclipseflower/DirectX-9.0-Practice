#include "PSMultiTexApp.h"


PSMultiTexApp::PSMultiTexApp(int width, int height) : InitApp(width, height)
{
}

bool PSMultiTexApp::Setup()
{
	device->CreateVertexBuffer(6 * sizeof(MultiTexVertex), D3DUSAGE_WRITEONLY, MultiTexVertex::FVF, D3DPOOL_MANAGED, &quadVB, 0);

	MultiTexVertex* v = 0;
	quadVB->Lock(0, 0, (void**)&v, 0);

	v[0] = MultiTexVertex(-10.0f, -10.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[1] = MultiTexVertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = MultiTexVertex(10.0f, 10.0f, 5.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = MultiTexVertex(-10.0f, -10.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[4] = MultiTexVertex(10.0f, 10.0f, 5.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = MultiTexVertex(10.0f, -10.0f, 5.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	quadVB->Unlock();

	ID3DXBuffer *shader = NULL;
	ID3DXBuffer *errorBuffer = NULL;

	D3DXCompileShaderFromFile("ps_multitex.hlsl", 0, 0, "Main", "ps_2_0", D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shader, &errorBuffer, &multiTexCT);

	if (errorBuffer)
	{
		MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		return false;
	}

	device->CreatePixelShader((DWORD *)shader->GetBufferPointer(), &multiTexPS);

	D3DXCreateTextureFromFile(device, "crate.bmp", &baseTex);
	D3DXCreateTextureFromFile(device, "spotlight.bmp", &spotLightTex);
	D3DXCreateTextureFromFile(device, "text.bmp", &stringTex);

	D3DXMATRIX P;
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI * 0.25f, (float)cfgWidth / (float)cfgHeight, 1.0f, 1000.0f);

	device->SetTransform(D3DTS_PROJECTION, &P);

	device->SetRenderState(D3DRS_LIGHTING, false);

	baseTexHandle = multiTexCT->GetConstantByName(0, "BaseTex");
	spotLightTexHandle = multiTexCT->GetConstantByName(0, "SpotLightTex");
	stringTexHandle = multiTexCT->GetConstantByName(0, "StringTex");

	UINT count;
	multiTexCT->GetConstantDesc(baseTexHandle, &baseTexDesc, &count);
	multiTexCT->GetConstantDesc(spotLightTexHandle, &spotLightTexDesc, &count);
	multiTexCT->GetConstantDesc(stringTexHandle, &stringTexDesc, &count);
	multiTexCT->SetDefaults(device);
	return true;
}

void PSMultiTexApp::Cleanup()
{
}

bool PSMultiTexApp::Display(float timeDelta)
{
	static float angle = (3.0f * D3DX_PI) / 2.0f;
	static float radius = 20.0f;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		angle -= 0.5f * timeDelta;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		angle += 0.5f * timeDelta;

	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		radius -= 2.0f * timeDelta;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		radius += 2.0f * timeDelta;

	D3DXVECTOR3 position(cosf(angle) * radius, 0.0f, sinf(angle) * radius);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	device->SetTransform(D3DTS_VIEW, &V);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	device->BeginScene();
	device->SetPixelShader(multiTexPS);
	device->SetFVF(MultiTexVertex::FVF);
	device->SetStreamSource(0, quadVB, 0, sizeof(MultiTexVertex));

	device->SetTexture(baseTexDesc.RegisterIndex, baseTex);
	device->SetSamplerState(baseTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(baseTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(baseTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	device->SetTexture(spotLightTexDesc.RegisterIndex, spotLightTex);
	device->SetSamplerState(spotLightTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(spotLightTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(spotLightTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	device->SetTexture(stringTexDesc.RegisterIndex, stringTex);
	device->SetSamplerState(stringTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(stringTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(stringTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

PSMultiTexApp::~PSMultiTexApp()
{
}
